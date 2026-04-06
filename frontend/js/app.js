// app.js — Cosmic simulator frontend

// ── API helpers ───────────────────────────────────────────────────────────────

const api = {
  async post(path, body) {
    const opts = { method: 'POST' };
    if (body instanceof Uint8Array) {
      opts.body = body;
    } else if (typeof body === 'string') {
      opts.body = body;
      opts.headers = { 'Content-Type': 'text/plain' };
    } else if (body) {
      opts.body = JSON.stringify(body);
      opts.headers = { 'Content-Type': 'application/json' };
    }
    const r = await fetch(path, opts);
    return r.json();
  },
  async get(path) {
    const r = await fetch(path);
    return r.json();
  },
};

// ── Formatting ────────────────────────────────────────────────────────────────

function hex8(n)  { return (n & 0xFF).toString(16).padStart(2, '0').toUpperCase(); }
function hex16(n) { return (n & 0xFFFF).toString(16).padStart(4, '0').toUpperCase(); }

// ── Application state ─────────────────────────────────────────────────────────

let cpuState  = null;   // last known cpu.State snapshot
let isRunning = false;
let pollTimer = null;

// ── Status display ────────────────────────────────────────────────────────────

function setStatus(running, halted) {
  const badge = document.getElementById('status-badge');
  isRunning = running;
  if (halted) {
    badge.textContent = 'HALTED';
    badge.className = 'badge badge-halted';
  } else if (running) {
    badge.textContent = 'RUNNING';
    badge.className = 'badge badge-running';
  } else {
    badge.textContent = 'STOPPED';
    badge.className = 'badge badge-stopped';
  }
  // Disable step while running; disable run while halted
  document.getElementById('btn-step').disabled = running;
  document.getElementById('btn-run').disabled  = running || halted;
  document.getElementById('btn-stop').disabled = !running;
}

// ── Register / flag display ───────────────────────────────────────────────────

function applyState(s) {
  cpuState = s;
  document.getElementById('reg-pc').textContent = hex16(s.pc);
  document.getElementById('reg-sp').textContent = hex16(s.sp);
  ['a','b','c','d','e','f','g','h'].forEach((name, i) => {
    document.getElementById('reg-' + name).textContent = hex8(s.registers[i]);
  });
  ['z','n','c','o','l','i','e'].forEach(f => {
    document.getElementById('flag-' + f).classList.toggle('set', !!s.flags[f]);
  });
}

// Handle responses from GET /api/state (wrapped) and POST handlers (flat).
function handleResponse(resp) {
  if (resp == null) return;
  if (resp.state !== undefined) {
    // GET /api/state → {state: {...}, running: bool}
    applyState(resp.state);
    setStatus(resp.running, resp.state.halted);
  } else if (resp.registers !== undefined) {
    // POST /api/step, /api/reset, /api/stop → flat snapshot
    applyState(resp);
    setStatus(false, resp.halted);
  }
}

// ── Memory editor (virtual scroll) ───────────────────────────────────────────
// Only ~80 rows are in the DOM at any time; the rest are rendered on demand as
// the user scrolls. This keeps the DOM small regardless of the 64 KB address space.

const MEM_ROW_H   = 20;   // px — must match CSS .mem-row height
const MEM_OVERSCAN = 10;  // extra rows rendered above/below the visible area

let _memData      = null;  // Uint8Array(65536) — full 64 KB cache
let _selectedAddr = null;  // address of the currently highlighted byte
let _renderStart  = -1;    // first row index currently in DOM
let _renderEnd    = -1;    // last row index (exclusive) currently in DOM

// Returns the named memory region for an address.
function memSection(addr) {
  if (addr < 0x8000) return 'Program';
  if (addr < 0xC000) return 'PGU Video';
  if (addr < 0xC800) return 'Stack';
  if (addr < 0xFFF2) return 'Variables';
  if (addr < 0xFFF4) return 'LPI Vector';
  if (addr < 0xFFF6) return 'HPI Vector';
  return 'Reserved';
}

// Updates the status bar. Pass null to show the idle dash.
function setMemStatus(addr) {
  const el = document.getElementById('memory-status');
  if (addr == null) { el.textContent = '—'; return; }
  const row = addr & ~0xF;
  const col = addr & 0xF;
  const val = _memData ? _memData[addr] : 0;
  el.textContent =
    `addr: ${hex16(addr)}  row: ${hex16(row)}  col: ${hex8(col)}` +
    `  val: 0x${hex8(val)} (${val})  [${memSection(addr)}]`;
}

// Scrolls the memory grid so the row containing addr is at the top.
function jumpToAddr(addr) {
  document.getElementById('memory-grid').scrollTop = (addr >> 4) * MEM_ROW_H;
}

// Builds the inner HTML for a single memory row.
function rowHTML(rowIdx) {
  const addr = rowIdx * 16;
  const pc = cpuState ? cpuState.pc : -1;
  let s = `<span class="mem-addr">${hex16(addr)}</span>`;
  for (let j = 0; j < 16; j++) {
    const abs = addr + j;
    const cls = abs === pc ? ' pc' : '';
    s += `<span class="mem-byte${cls}" id="mb-${abs}">${hex8(_memData[abs])}</span>`;
  }
  return s;
}

// Renders only the rows currently visible (plus overscan). Called on scroll and
// whenever data or CPU state changes. Skips work if the visible range hasn't changed.
function renderVisible() {
  if (!_memData) return;
  const grid = document.getElementById('memory-grid');
  const rows = document.getElementById('mem-rows');
  if (!grid || !rows) return;

  const st    = grid.scrollTop;
  const ch    = grid.clientHeight || 400;
  const first = Math.max(0, Math.floor(st / MEM_ROW_H) - MEM_OVERSCAN);
  const last  = Math.min(4096, Math.ceil((st + ch) / MEM_ROW_H) + MEM_OVERSCAN);

  if (first === _renderStart && last === _renderEnd) return;
  _renderStart = first;
  _renderEnd   = last;

  rows.style.top = (first * MEM_ROW_H) + 'px';
  const parts = [];
  for (let r = first; r < last; r++) {
    parts.push(`<div class="mem-row">${rowHTML(r)}</div>`);
  }
  rows.innerHTML = parts.join('');
}

// One-time setup: column header, spacer height, scroll listener.
function initMemGrid() {
  const hdr = document.getElementById('memory-col-header');
  let h = '<span class="mem-addr"></span>';
  for (let j = 0; j < 16; j++) h += `<span class="mem-col-hdr">${hex8(j)}</span>`;
  hdr.innerHTML = h;

  document.getElementById('mem-spacer').style.height = (4096 * MEM_ROW_H) + 'px';
  document.getElementById('memory-grid').addEventListener('scroll', renderVisible);
}

// Fetches all 64 KB from the server and refreshes the view.
// Pass rebuild=true to also scroll back to address 0000.
async function fetchMemory(rebuild = false) {
  const resp = await api.get('/api/memory?from=0000&to=ffff');
  const firstLoad = !_memData;
  _memData = new Uint8Array(resp.data);

  if (firstLoad) {
    initMemGrid();
    renderVisible();
  } else {
    if (rebuild) document.getElementById('memory-grid').scrollTop = 0;
    _renderStart = _renderEnd = -1;  // invalidate cache → force re-render
    renderVisible();
  }
}

// Opens an inline hex editor for the clicked byte span.
function startByteEdit(span) {
  if (isRunning) return;
  const addr = parseInt(span.id.slice(3)); // strip 'mb-' prefix
  _selectedAddr = addr;
  setMemStatus(addr);

  const inp = document.createElement('input');
  inp.type      = 'text';
  inp.maxLength = 2;
  inp.className = 'mem-byte-input';
  inp.value     = span.textContent;
  inp.style.width = '2ch';

  span.replaceWith(inp);
  inp.select();

  async function commit() {
    const val  = inp.value.trim();
    const byte = parseInt(val, 16);
    if (val.length > 0 && !isNaN(byte) && byte >= 0 && byte <= 0xFF) {
      await api.post('/api/memory', { address: hex16(addr), data: [byte] });
    }
    await fetchMemory();
  }

  inp.addEventListener('keydown', async ev => {
    if (ev.key === 'Enter')  { ev.preventDefault(); await commit(); }
    if (ev.key === 'Escape') { await fetchMemory(); }
  });
  inp.addEventListener('blur', commit);
}

// Event delegation: one set of listeners on the grid container covers all byte spans.
(function () {
  const grid = document.getElementById('memory-grid');
  grid.addEventListener('mouseover', e => {
    const el = e.target.closest('.mem-byte');
    if (el) setMemStatus(parseInt(el.id.slice(3)));
  });
  grid.addEventListener('mouseleave', () => setMemStatus(_selectedAddr));
  grid.addEventListener('click', e => {
    const el = e.target.closest('.mem-byte');
    if (el && el.tagName === 'SPAN') startByteEdit(el);
  });
}());

// ── Control buttons ───────────────────────────────────────────────────────────

document.getElementById('btn-run').addEventListener('click', async () => {
  const hz = document.getElementById('inp-hz').value.trim();
  const path = hz ? `/api/run?hz=${hz}` : '/api/run';
  await api.post(path);
  setStatus(true, false);
  startPolling();
});

document.getElementById('btn-stop').addEventListener('click', async () => {
  stopPolling();
  const resp = await api.post('/api/stop');
  handleResponse(resp);
  await fetchMemory();
});

document.getElementById('btn-reset').addEventListener('click', async () => {
  stopPolling();
  const resp = await api.post('/api/reset');
  handleResponse(resp);
  await fetchMemory(true);
});

document.getElementById('btn-step').addEventListener('click', async () => {
  const resp = await api.post('/api/step');
  handleResponse(resp);
  await fetchMemory();
});

document.getElementById('btn-assemble').addEventListener('click', async () => {
  const src = document.getElementById('editor').value;
  const out = document.getElementById('assembler-output');
  const result = await api.post('/api/assemble', src);
  if (result.error) {
    out.textContent = 'Error: ' + result.error;
    out.className = 'output-error';
  } else {
    out.textContent = `OK — ${result.bytes} bytes loaded.` +
      (result.warnings?.length ? '\n' + result.warnings.join('\n') : '');
    out.className = 'output-ok';
    await fetchMemory(true);
  }
});

document.getElementById('btn-load-binary').addEventListener('change', async (e) => {
  const file = e.target.files[0];
  if (!file) return;
  const buf = await file.arrayBuffer();
  const result = await api.post('/api/load', new Uint8Array(buf));
  const out = document.getElementById('assembler-output');
  out.textContent = result.error ? 'Error: ' + result.error : `Loaded ${file.name}`;
  out.className = result.error ? 'output-error' : 'output-ok';
  await fetchMemory(true);
  e.target.value = '';  // allow re-loading same file
});

// Memory jump controls
document.getElementById('btn-mem-jump').addEventListener('click', () => {
  const addr = parseInt(document.getElementById('mem-jump').value, 16) || 0;
  jumpToAddr(addr);
});
document.getElementById('mem-jump').addEventListener('keydown', e => {
  if (e.key === 'Enter') document.getElementById('btn-mem-jump').click();
});

document.getElementById('btn-mem-pc').addEventListener('click', () => {
  if (cpuState) jumpToAddr(cpuState.pc);
});

document.getElementById('btn-mem-refresh').addEventListener('click', async () => {
  _memData = null;
  await fetchMemory();
});

document.getElementById('btn-mem-dump').addEventListener('click', () => {
  const a = document.createElement('a');
  a.href = '/api/memory/dump?from=0000&to=ffff';
  a.download = 'memory_0000_ffff.bin';
  a.click();
});

document.querySelectorAll('.btn-region').forEach(btn => {
  btn.addEventListener('click', () => {
    jumpToAddr(parseInt(btn.dataset.addr, 16));
  });
});

// ── Polling (replaces WebSocket until WS is implemented) ─────────────────────

async function pollState() {
  const resp = await api.get('/api/state');
  handleResponse(resp);
  if (!resp.running) {
    // CPU stopped (halted or manual stop)
    stopPolling();
    await fetchMemory();
  }
}

function startPolling() {
  if (pollTimer) return;
  pollTimer = setInterval(pollState, 150);
}

function stopPolling() {
  if (pollTimer) { clearInterval(pollTimer); pollTimer = null; }
}

// ── PGU canvas (placeholder until WebSocket delivers frames) ──────────────────

function renderPGUFrame(base64) {
  const canvas = document.getElementById('pgu-canvas');
  const ctx = canvas.getContext('2d');
  const img = new Image();
  img.onload = () => ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
  img.src = 'data:image/png;base64,' + base64;
}

// ── Init ──────────────────────────────────────────────────────────────────────

(async () => {
  const resp = await api.get('/api/state');
  handleResponse(resp);
  await fetchMemory();
})();

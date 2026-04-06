import { useState, useRef, useCallback } from 'react';
import { useVirtualizer } from '@tanstack/react-virtual';
import { useCosmic } from '../context/CosmicContext';
import { hex8, hex16 } from '../api';

function memSection(addr) {
  if (addr < 0x8000) return 'Program';
  if (addr < 0xC000) return 'PGU Video';
  if (addr < 0xC800) return 'Stack';
  if (addr < 0xFFF2) return 'Variables';
  if (addr < 0xFFF4) return 'LPI Vector';
  if (addr < 0xFFF6) return 'HPI Vector';
  return 'Reserved';
}

function EditCell({ addr, initialValue, onDone }) {
  const [val, setVal] = useState(initialValue);
  const done = useRef(false);

  async function commit() {
    if (done.current) return;
    done.current = true;
    const byte = parseInt(val.trim(), 16);
    const valid = val.trim().length > 0 && !isNaN(byte) && byte >= 0 && byte <= 0xFF;
    onDone(valid ? byte : null);
  }

  return (
    <input
      className="mem-byte-input"
      maxLength={2}
      value={val}
      style={{ width: '2ch' }}
      autoFocus
      onChange={e => setVal(e.target.value)}
      onKeyDown={async e => {
        if (e.key === 'Enter') { e.preventDefault(); await commit(); }
        if (e.key === 'Escape') { done.current = true; onDone(null); }
      }}
      onBlur={commit}
    />
  );
}

const REGIONS = [
  { label: 'Program',   addr: 0x0000, title: 'General program space: 0000–7FFF' },
  { label: 'Video',     addr: 0x8000, title: 'PGU video memory: 8000–BFFF' },
  { label: 'Stack',     addr: 0xC000, title: 'Stack area: C000–C7FF' },
  { label: 'Variables', addr: 0xC800, title: 'Variable / data space: C800–FFF1' },
  { label: 'Vectors',   addr: 0xFFF0, title: 'LPI vector: FFF2–FFF3 · HPI vector: FFF4–FFF5' },
];

export default function MemoryPanel() {
  const { memData, cpuState, writeByte, isRunning, fetchMemory, resetMemory } = useCosmic();
  const [hoverAddr, setHoverAddr] = useState(null);
  const [selectedAddr, setSelectedAddr] = useState(null);
  const [editingAddr, setEditingAddr] = useState(null);
  const [jumpInput, setJumpInput] = useState('0000');
  const parentRef = useRef(null);

  const rowVirtualizer = useVirtualizer({
    count: 4096,
    getScrollElement: () => parentRef.current,
    estimateSize: () => 20,
    overscan: 10,
  });

  function jump(addr) {
    if (parentRef.current) parentRef.current.scrollTop = (addr >> 4) * 20;
  }

  async function handleEditDone(addr, byte) {
    setEditingAddr(null);
    if (byte !== null) await writeByte(addr, byte);
    else await fetchMemory();
  }

  function handleByteClick(addr) {
    if (isRunning) return;
    setSelectedAddr(addr);
    setEditingAddr(addr);
  }

  const displayAddr = hoverAddr ?? selectedAddr;
  let statusText = '—';
  if (displayAddr != null && memData) {
    const val = memData[displayAddr];
    statusText = `addr: ${hex16(displayAddr)}  row: ${hex16(displayAddr & ~0xF)}  col: ${hex8(displayAddr & 0xF)}  val: 0x${hex8(val)} (${val})  [${memSection(displayAddr)}]`;
  }

  const pc = cpuState?.pc ?? -1;

  return (
    <div className="panel-memory">
      <div id="memory-header">
        <h2>Memory</h2>
        <div id="memory-controls">
          <input
            value={jumpInput}
            maxLength={4}
            size={4}
            onChange={e => setJumpInput(e.target.value)}
            onKeyDown={e => e.key === 'Enter' && jump(parseInt(jumpInput, 16) || 0)}
            title="Hex address to jump to"
          />
          <button onClick={() => jump(parseInt(jumpInput, 16) || 0)}>Go</button>
          <button onClick={() => cpuState && jump(cpuState.pc)}>PC</button>
          <button onClick={fetchMemory}>Refresh</button>
          <button onClick={() => {
            const a = document.createElement('a');
            a.href = '/api/memory/dump?from=0000&to=ffff';
            a.download = 'mem.bin';
            a.click();
          }}>Dump</button>
          <button onClick={resetMemory}>Reset</button>
        </div>
        <div id="memory-regions">
          {REGIONS.map(r => (
            <button key={r.addr} className="btn-region" title={r.title} onClick={() => jump(r.addr)}>
              {r.label}
            </button>
          ))}
        </div>
      </div>

      <div id="memory-col-header" className="mem-row">
        <span className="mem-addr" />
        {Array.from({ length: 16 }, (_, j) => (
          <span key={j} className="mem-col-hdr">{hex8(j)}</span>
        ))}
      </div>

      <div
        id="memory-grid"
        ref={parentRef}
        onMouseLeave={() => setHoverAddr(null)}
      >
        <div style={{ height: rowVirtualizer.getTotalSize(), position: 'relative' }}>
          {rowVirtualizer.getVirtualItems().map(vRow => {
            const baseAddr = vRow.index * 16;
            return (
              <div
                key={vRow.index}
                className="mem-row"
                style={{
                  position: 'absolute',
                  top: vRow.start,
                  left: 0,
                  right: 0,
                  height: vRow.size,
                  padding: '0 0.75rem',
                }}
              >
                <span className="mem-addr">{hex16(baseAddr)}</span>
                {Array.from({ length: 16 }, (_, j) => {
                  const abs = baseAddr + j;
                  if (abs === editingAddr) {
                    return (
                      <EditCell
                        key={abs}
                        addr={abs}
                        initialValue={memData ? hex8(memData[abs]) : '00'}
                        onDone={byte => handleEditDone(abs, byte)}
                      />
                    );
                  }
                  return (
                    <span
                      key={abs}
                      className={`mem-byte${abs === pc ? ' pc' : ''}`}
                      onClick={() => handleByteClick(abs)}
                      onMouseEnter={() => setHoverAddr(abs)}
                    >
                      {memData ? hex8(memData[abs]) : '00'}
                    </span>
                  );
                })}
              </div>
            );
          })}
        </div>
      </div>

      <div id="memory-status">{statusText}</div>
    </div>
  );
}

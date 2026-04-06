export const api = {
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
  async delete(path) {
    const opts = { method: 'DELETE' };
    const r = await fetch(path, opts);
    return r.json();
  }
};

export function hex8(n)  { return (n & 0xFF).toString(16).padStart(2, '0').toUpperCase(); }
export function hex16(n) { return (n & 0xFFFF).toString(16).padStart(4, '0').toUpperCase(); }

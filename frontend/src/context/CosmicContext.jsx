import { createContext, useContext, useState, useEffect, useRef, useCallback } from 'react';
import { api, hex16 } from '../api';

const CosmicContext = createContext(null);

export function CosmicProvider({ children }) {
  const [cpuState, setCpuState] = useState(null);
  const [isRunning, setIsRunning] = useState(false);
  const [memData, setMemData] = useState(null);
  const pollTimer = useRef(null);

  const fetchMemory = useCallback(async () => {
    const resp = await api.get('/api/memory?from=0000&to=ffff');
    setMemData(new Uint8Array(resp.data));
  }, []);

  const applyResponse = useCallback((resp) => {
    if (!resp) return;
    if (resp.state !== undefined) {
      setCpuState(resp.state);
      setIsRunning(resp.running);
    } else if (resp.registers !== undefined) {
      setCpuState(resp);
      setIsRunning(false);
    }
  }, []);

  const stopPolling = useCallback(() => {
    if (pollTimer.current) { clearInterval(pollTimer.current); pollTimer.current = null; }
  }, []);

  const startPolling = useCallback(() => {
    if (pollTimer.current) return;
    pollTimer.current = setInterval(async () => {
      const resp = await api.get('/api/state');
      applyResponse(resp);
      if (!resp.running) { stopPolling(); await fetchMemory(); }
    }, 150);
  }, [applyResponse, stopPolling, fetchMemory]);

  const run = useCallback(async (hz) => {
    const path = hz ? `/api/run?hz=${hz}` : '/api/run';
    await api.post(path);
    setIsRunning(true);
    startPolling();
  }, [startPolling]);

  const stop = useCallback(async () => {
    stopPolling();
    const resp = await api.post('/api/stop');
    applyResponse(resp);
    await fetchMemory();
  }, [stopPolling, applyResponse, fetchMemory]);

  const reset = useCallback(async () => {
    stopPolling();
    const resp = await api.post('/api/reset');
    applyResponse(resp);
    await fetchMemory();
  }, [stopPolling, applyResponse, fetchMemory]);

  const resetMemory = useCallback(async () => {
    stopPolling();
    const resp = await api.post('/api/resetmemory');
    applyResponse(resp)
    await fetchMemory();
  }, [stopPolling, applyResponse, fetchMemory]);

  const step = useCallback(async () => {
    const resp = await api.post('/api/step');
    applyResponse(resp);
    await fetchMemory();
  }, [applyResponse, fetchMemory]);

  const writeByte = useCallback(async (addr, byte) => {
    await api.post('/api/memory', { address: hex16(addr), data: [byte] });
    await fetchMemory();
  }, [fetchMemory]);

  const assemble = useCallback(async (src) => {
    const result = await api.post('/api/assemble', src);
    if (!result.error) await fetchMemory();
    return result;
  }, [fetchMemory]);

  const loadBinary = useCallback(async (bytes) => {
    const result = await api.post('/api/load', bytes);
    if (!result.error) await fetchMemory();
    return result;
  }, [fetchMemory]);

  useEffect(() => {
    (async () => {
      const resp = await api.get('/api/state');
      applyResponse(resp);
      await fetchMemory();
    })();
    return () => stopPolling();
  }, []); // eslint-disable-line

  const halted = cpuState?.halted ?? false;

  return (
    <CosmicContext.Provider value={{
      cpuState, isRunning, halted, memData,
      run, stop, reset, resetMemory, step, writeByte, assemble, loadBinary, fetchMemory,
    }}>
      {children}
    </CosmicContext.Provider>
  );
}

export function useCosmic() {
  return useContext(CosmicContext);
}

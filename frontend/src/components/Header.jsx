import { useState } from 'react';
import { useCosmic } from '../context/CosmicContext';

export default function Header() {
  const { isRunning, halted, run, stop, reset, step } = useCosmic();
  const [hz, setHz] = useState('');

  let badgeText, badgeClass;
  if (halted)       { badgeText = 'HALTED';  badgeClass = 'badge-halted'; }
  else if (isRunning) { badgeText = 'RUNNING'; badgeClass = 'badge-running'; }
  else              { badgeText = 'STOPPED'; badgeClass = 'badge-stopped'; }

  return (
    <header>
      <h1>Cosmic</h1>
      <div id="controls">
        <button onClick={() => run(hz || undefined)} disabled={isRunning || halted}>Run</button>
        <input
          id="inp-hz"
          type="number"
          min="1"
          placeholder="Hz"
          value={hz}
          onChange={e => setHz(e.target.value)}
          title="Clock speed in Hz (leave empty for max)"
        />
        <button onClick={stop}  disabled={!isRunning}>Stop</button>
        <button onClick={step}  disabled={isRunning}>Step</button>
        <button onClick={reset}>Reset</button>
      </div>
      <span className={`badge ${badgeClass}`}>{badgeText}</span>
    </header>
  );
}

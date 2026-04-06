import { useState } from 'react';
import { useCosmic } from '../context/CosmicContext';

export default function EditorPanel() {
  const { assemble, loadBinary } = useCosmic();
  const [src, setSrc] = useState('');
  const [output, setOutput] = useState({ text: '', status: '' });

  async function handleAssemble() {
    const result = await assemble(src);
    if (result.error) {
      setOutput({ text: 'Error: ' + result.error, status: 'error' });
    } else {
      const w = result.warnings?.length ? '\n' + result.warnings.join('\n') : '';
      setOutput({ text: `OK — ${result.bytes} bytes loaded.${w}`, status: 'ok' });
    }
  }

  async function handleLoadBinary(e) {
    const file = e.target.files[0];
    if (!file) return;
    const result = await loadBinary(new Uint8Array(await file.arrayBuffer()));
    setOutput(result.error
      ? { text: 'Error: ' + result.error, status: 'error' }
      : { text: `Loaded ${file.name}`, status: 'ok' });
    e.target.value = '';
  }

  return (
    <div className="panel-editor">
      <h2>Assembly Editor</h2>
      <textarea
        id="editor"
        value={src}
        onChange={e => setSrc(e.target.value)}
        spellCheck={false}
        placeholder="; Enter Cosmic assembly here..."
      />
      <div id="editor-controls">
        <button onClick={handleAssemble}>Assemble &amp; Load</button>
        <label className="file-label">
          Load Binary
          <input type="file" accept=".bin" onChange={handleLoadBinary} style={{ display: 'none' }} />
        </label>
      </div>
      <pre className={`assembler-output${output.status ? ' output-' + output.status : ''}`}>
        {output.text}
      </pre>
    </div>
  );
}

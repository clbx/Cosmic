import { DockviewReact } from 'dockview';
import { CosmicProvider } from './context/CosmicContext';
import Header from './components/Header';
import EditorPanel from './components/EditorPanel';
import RegistersPanel from './components/RegistersPanel';
import MemoryPanel from './components/MemoryPanel';
import VideoOutputPanel from './components/VideoOutputPanel';
import 'dockview/dist/styles/dockview.css';

const components = {
  editor:    EditorPanel,
  registers: RegistersPanel,
  memory:    MemoryPanel,
  videoOutput: VideoOutputPanel,
};

function onReady(event) {
  const api = event.api;
  api.addPanel({ id: 'editor',    component: 'editor',    title: 'Assembly Editor' });
  api.addPanel({ id: 'registers', component: 'registers', title: 'Registers',
    position: { referencePanel: 'editor', direction: 'right' } });
  api.addPanel({ id: 'memory',    component: 'memory',    title: 'Memory',
    position: { referencePanel: 'registers', direction: 'right' } });
  api.addPanel({ id: 'videoOutput', component: 'videoOutput', title: 'Video Output',
    position: { referencePanel: 'registers', direction: 'below'}});
}

export default function App() {
  return (
    <CosmicProvider>
      <div id="app-root">
        <Header />
        <DockviewReact
          className="dockview-theme-dark"
          components={components}
          onReady={onReady}
        />
      </div>
    </CosmicProvider>
  );
}

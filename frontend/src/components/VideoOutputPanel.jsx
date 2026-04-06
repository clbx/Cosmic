import { useCosmic } from '../context/CosmicContext';
import { hex8, hex16 } from '../api';


export default function VideoOutputPanel() {
    const { cpuState } = useCosmic();
    const s = cpuState;

    return (
        <div className='panel-videoOutput'>
            <h2>Video Output</h2>
            <canvas id="pgu-canvas" width="300" height="200" />
        </div>
    )
}
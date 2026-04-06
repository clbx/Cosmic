import { useCosmic } from '../context/CosmicContext';
import { hex8, hex16 } from '../api';

const REG_TITLES = {
  PC: 'Program Counter — address of the next instruction to execute',
  SP: 'Stack Pointer — address of the top of the stack',
  A:  'Accumulator — destination for arithmetic and logic operations',
  B:  'General purpose — holds remainder after DIV/DIVX',
  C:  'General purpose — high byte of 16-bit pair CD',
  D:  'General purpose — low byte of 16-bit pair CD',
  E:  'General purpose — high byte of 16-bit pair EF',
  F:  'General purpose — low byte of 16-bit pair EF',
  G:  'General purpose — high byte of 16-bit pair GH',
  H:  'General purpose — low byte of 16-bit pair GH',
};

const FLAG_TITLES = {
  Z: 'Zero — result was zero',
  N: 'Negative — result was negative (bit 7 set)',
  C: 'Carry — unsigned overflow or borrow occurred',
  O: 'Overflow — signed overflow occurred',
  L: 'Less — signed comparison: accumulator was less than operand',
  I: 'Interrupt disable — low-priority interrupts are masked',
  E: 'Error — divide by zero occurred',
};

export default function RegistersPanel() {
  const { cpuState } = useCosmic();
  const s = cpuState;

  return (
    <div className="panel-registers">
      <h2>Registers</h2>
      <table id="reg-table">
        <tbody>
          <tr title={REG_TITLES.PC}><th>PC</th><td>{s ? hex16(s.pc) : '0000'}</td></tr>
          <tr title={REG_TITLES.SP}><th>SP</th><td>{s ? hex16(s.sp) : 'C399'}</td></tr>
          {['A','B','C','D','E','F','G','H'].map((name, i) => (
            <tr key={name} title={REG_TITLES[name]}>
              <th>{name}</th>
              <td>{s ? hex8(s.registers[i]) : '00'}</td>
            </tr>
          ))}
        </tbody>
      </table>

      <h2>Flags</h2>
      <div id="flags">
        {['Z','N','C','O','L','I','E'].map(f => (
          <span
            key={f}
            className={`flag${s?.flags?.[f.toLowerCase()] ? ' set' : ''}`}
            title={FLAG_TITLES[f]}
          >{f}</span>
        ))}
      </div>
    </div>
  );
}

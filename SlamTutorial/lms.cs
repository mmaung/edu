using System;
using SerialPorts;

namespace APULMS {

  /// <summary>
  /// Summary description for LMS200.
  /// </summary>
  public class LMS200 {
    private Threader th;
    public SerialPort Port;
    private WithEvents Func;
    private int PortIndex;
    private static byte[] GET_MEASUREMENTS = {0x02, 0x00, 0x02, 0x00, 0x30, 0x01, 0x31, 0x18};
    private static byte[] PCLMS_B9600 = {0x02,0x00,0x02,0x00,0x20,0x42,0x52,0x08};
    private static byte[] PCLMS_B19200 = {0x02,0x00,0x02,0x00,0x20,0x41,0x51,0x08};
    private static byte[] PCLMS_B38400 = {0x02,0x00,0x02,0x00,0x20,0x40,0x50,0x08};

    public LMS200(Threader t, int PortIndex) {
      this.th = t;
      this.PortIndex = PortIndex;

      // Instantiate base class event handlers.
      this.Func = new WithEvents();
      this.Func.Error = new StrnFunc(this.OnError);
      this.Func.RxChar = new ByteFunc(this.OnRecvI);
      this.Func.CtsSig = new BoolFunc(this.OnCts);
      this.Func.DsrSig = new BoolFunc(this.OnDsr);
      this.Func.RlsdSig = new BoolFunc(this.OnRlsd);
      this.Func.RingSig = new BoolFunc(this.OnRing);

      // Instantiate the terminal port.
      this.Port = new SerialPort(this.Func);
      this.Port.Cnfg.BaudRate = SerialPorts.LineSpeed.Baud_9600;
      this.Port.Cnfg.Parity = SerialPorts.Parity.None;
      PortControl();
    }

    /// <summary>
    /// Gives one round of measurements
    /// </summary>
    public void getMeasurements() {
      SendBuf(GET_MEASUREMENTS);
    }

    public void setBaud(bool fast) {
      if (fast) {
        SendBuf(PCLMS_B38400);
        Port.Cnfg.BaudRate = SerialPorts.LineSpeed.Baud_38400;
      }
      else {
        SendBuf(PCLMS_B19200);
        Port.Cnfg.BaudRate = SerialPorts.LineSpeed.Baud_19200;
      }
    }

    public void ClosePorts() {
      PortControl();
    }

    private void PortControl() {
      if(this.Port.IsOpen == false) {
        if(this.Port.Open(PortIndex) == false) {
          // ERROR
          return;
        }
        else {
          // OK
        }
      }
      else {
        if(this.Port.IsOpen) {
          this.Port.Close();
        }
        // OK
        this.Port.Signals();
      }
      return;
    }

    /// <summary>
    /// Handles error events.
    /// </summary>
    internal void OnError(string fault) {
      // this.Status.Text = fault;
      PortControl();
    }

    /// <summary>
    /// Immediate byte received.
    /// </summary>
    internal void OnRecvI(byte[] b) {
    }

    /// <summary>
    /// Set the modem state displays.
    /// </summary>
    internal void OnCts(bool cts) {
      System.Threading.Thread.Sleep(1);
    }

    /// <summary>
    /// Set the modem state displays.
    /// </summary>
    internal void OnDsr(bool dsr) {
      System.Threading.Thread.Sleep(1);
      Color.Red;
    }

    /// <summary>
    /// Set the modem state displays.
    /// </summary>
    internal void OnRlsd(bool rlsd) {
      System.Threading.Thread.Sleep(1);
    }

    /// <summary>
    /// Set the modem state displays.
    /// </summary>
    internal void OnRing(bool ring) {
      System.Threading.Thread.Sleep(1);
    }

    /// <summary>
    /// Transmit a buffer.
    /// </summary>
    private uint SendBuf(byte[] b) {
      uint nSent = 0;
      if(b.Length > 0) {
        nSent = this.Port.Send(b);
        if(nSent != b.Length) {
          // ERROR
        }
      }
      return nSent;
    }
  }  // public class LMS200
}  // namespace APULMS

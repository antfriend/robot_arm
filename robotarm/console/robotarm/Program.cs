using System;
using System.IO.Ports;
using System.Threading;
using System.Drawing;
using Colorful;
using Console = Colorful.Console;

namespace robotarm
{
    class Program
    {
        static SerialPort _serialPort;
        static int _current_action_index = 0;
        static string[] _actionables = new string[] {"claw","wrist","elbow","knee","hips","base"};

        static void Main(string[] args)
        {
            _serialPort = new SerialPort();
            _serialPort.PortName = "COM6";
            _serialPort.BaudRate = 9600;
            _serialPort.Open();
            say("WHAT?!?!");
            //another visitor ...
            //stay awhile
            string bloop = string.Empty;
            while (true)//stay FOREVER!!!
            {
                bloop = keyReaderLoop(bloop);
            }
        }

        private static Color ColorByNumber(int theNumber)
        {
            switch (theNumber)
            {
                case 1: return Color.Red;
                case 2: return Color.Yellow;
                case 3: return Color.Green;
                case 4: return Color.Black;
                case 5: return Color.White;
                default: return Color.Black;
            }
        }

        private static void Figlet(string strang)
        {
            FigletFont font = FigletFont.Load(@"fonts\cosmic.flf");//1995 http://www.figlet.org/fontdb.cgi
            Figlet figlet = new Figlet(font);
            Console.WriteLine(figlet.ToAscii(strang), ColorByNumber(1));
        }

        private static void say(string writeThis){
            Console.Clear();
            Figlet(writeThis);
            //Console.Write($"{writeThis}     ");
        }

        private static void send(string blurb){
            _serialPort.WriteLine(blurb);
        }

        private static string keyReaderLoop(string blurb)
        {
            ConsoleKeyInfo k = System.Console.ReadKey();
            switch (k.Key)
            {
                case ConsoleKey.Backspace:
                    break;
                case ConsoleKey.Tab:
                    break;
                case ConsoleKey.Clear:
                    break;
                case ConsoleKey.Enter:
                    send(blurb);
                    //speak($"sending {blurb}");
                    Thread.Sleep(200);
                    blurb = string.Empty;
                    break;
                case ConsoleKey.Pause:
                    break;
                case ConsoleKey.Escape:
                    break;
                case ConsoleKey.Spacebar:
                    break;
                case ConsoleKey.PageUp:
                    say("turn");
                    blurb = "k";
                    send(blurb);
                    break;
                case ConsoleKey.PageDown:
                    say("turn");
                    blurb = "l";
                    send(blurb);
                    break;
                case ConsoleKey.End:
                    say("zzzz");
                    blurb = "r";
                    send(blurb);
                    break;

                case ConsoleKey.Home:
                    say("park");
                    blurb = "a";
                    send(blurb);
                    break;

                case ConsoleKey.LeftArrow:
                    Console.Write("<");
                    action_left();
                    break;

                case ConsoleKey.RightArrow:
                    Console.Write(">");
                    action_right();
                    break;

                case ConsoleKey.UpArrow:
                    set_current_up();
                    break;

                case ConsoleKey.DownArrow:
                    set_current_down();
                    break;
                case ConsoleKey.Select:
                    break;
                case ConsoleKey.Print:
                    break;
                case ConsoleKey.Execute:
                    break;
                case ConsoleKey.PrintScreen:
                    break;
                case ConsoleKey.Insert:
                    break;
                case ConsoleKey.Delete:
                    Console.Beep(440,3);
                    say("beep");
                    break;
                case ConsoleKey.Help:
                    break;
                case ConsoleKey.D0:
                    break;
                case ConsoleKey.D1:
                    break;
                case ConsoleKey.D2:
                    break;
                case ConsoleKey.D3:
                    break;
                case ConsoleKey.D4:
                    break;
                case ConsoleKey.D5:
                    break;
                case ConsoleKey.D6:
                    break;
                case ConsoleKey.D7:
                    break;
                case ConsoleKey.D8:
                    break;
                case ConsoleKey.D9:
                    break;

                
                case ConsoleKey.B:
                case ConsoleKey.C:
                case ConsoleKey.D:
                case ConsoleKey.E:
                case ConsoleKey.F:
                case ConsoleKey.G:
                case ConsoleKey.H:
                case ConsoleKey.I:
                case ConsoleKey.J:
                case ConsoleKey.K:
                case ConsoleKey.L:
                case ConsoleKey.M:
                case ConsoleKey.N:
                case ConsoleKey.O:
                
                case ConsoleKey.Q:
                
                
                case ConsoleKey.T:
                case ConsoleKey.U:
                case ConsoleKey.V:
                case ConsoleKey.W:
                case ConsoleKey.X:
                case ConsoleKey.Y:
                case ConsoleKey.Z:
                    return $"{blurb}{k.Key}";
                    //break;
                case ConsoleKey.S:
                    say("stand");
                    blurb = "s";
                    send(blurb);
                    break;
                case ConsoleKey.R:
                    say("rest ");
                    blurb = "r";
                    send(blurb);
                    break;
                case ConsoleKey.A:
                    say("activate");
                    blurb = "a";
                    send(blurb);
                    break;
                case ConsoleKey.P:
                    say("park");
                    blurb = "p";
                    send(blurb);
                    break;                    
                case ConsoleKey.LeftWindows:
                    break;
                case ConsoleKey.RightWindows:
                    break;
                case ConsoleKey.Applications:
                    break;
                case ConsoleKey.Sleep:
                    break;
                case ConsoleKey.NumPad0:
                    break;
                case ConsoleKey.NumPad1:
                    break;
                case ConsoleKey.NumPad2:
                    break;
                case ConsoleKey.NumPad3:
                    break;
                case ConsoleKey.NumPad4:
                    break;
                case ConsoleKey.NumPad5:
                    break;
                case ConsoleKey.NumPad6:
                    break;
                case ConsoleKey.NumPad7:
                    break;
                case ConsoleKey.NumPad8:
                    break;
                case ConsoleKey.NumPad9:
                    break;
                case ConsoleKey.Multiply:
                    break;
                case ConsoleKey.Add:
                    break;
                case ConsoleKey.Separator:
                    break;
                case ConsoleKey.Subtract:
                    break;
                case ConsoleKey.Decimal:
                    break;
                case ConsoleKey.Divide:
                    break;
                case ConsoleKey.F1:
                    break;
                case ConsoleKey.F2:
                    break;
                case ConsoleKey.F3:
                    break;
                case ConsoleKey.F4:
                    break;
                case ConsoleKey.F5:
                    break;
                case ConsoleKey.F6:
                    break;
                case ConsoleKey.F7:
                    break;
                case ConsoleKey.F8:
                    break;
                case ConsoleKey.F9:
                    break;
                case ConsoleKey.F10:
                    break;
                case ConsoleKey.F11:
                    break;
                case ConsoleKey.F12:
                    break;
                case ConsoleKey.F13:
                    break;
                case ConsoleKey.F14:
                    break;
                case ConsoleKey.F15:
                    break;
                case ConsoleKey.F16:
                    break;
                case ConsoleKey.F17:
                    break;
                case ConsoleKey.F18:
                    break;
                case ConsoleKey.F19:
                    break;
                case ConsoleKey.F20:
                    break;
                case ConsoleKey.F21:
                    break;
                case ConsoleKey.F22:
                    break;
                case ConsoleKey.F23:
                    break;
                case ConsoleKey.F24:
                    break;
                case ConsoleKey.BrowserBack:
                    break;
                case ConsoleKey.BrowserForward:
                    break;
                case ConsoleKey.BrowserRefresh:
                    break;
                case ConsoleKey.BrowserStop:
                    break;
                case ConsoleKey.BrowserSearch:
                    break;
                case ConsoleKey.BrowserFavorites:
                    break;
                case ConsoleKey.BrowserHome:
                    break;
                case ConsoleKey.VolumeMute:
                    break;
                case ConsoleKey.VolumeDown:
                    break;
                case ConsoleKey.VolumeUp:
                    break;
                case ConsoleKey.MediaNext:
                    break;
                case ConsoleKey.MediaPrevious:
                    break;
                case ConsoleKey.MediaStop:
                    break;
                case ConsoleKey.MediaPlay:
                    break;
                case ConsoleKey.LaunchMail:
                    break;
                case ConsoleKey.LaunchMediaSelect:
                    break;
                case ConsoleKey.LaunchApp1:
                    break;
                case ConsoleKey.LaunchApp2:
                    break;
                case ConsoleKey.Oem1:
                    break;
                case ConsoleKey.OemPlus:
                    break;
                case ConsoleKey.OemComma:
                    break;
                case ConsoleKey.OemMinus:
                    break;
                case ConsoleKey.OemPeriod:
                    break;
                case ConsoleKey.Oem2:
                    break;
                case ConsoleKey.Oem3:
                    break;
                case ConsoleKey.Oem4:
                    break;
                case ConsoleKey.Oem5:
                    break;
                case ConsoleKey.Oem6:
                    break;
                case ConsoleKey.Oem7:
                    break;
                case ConsoleKey.Oem8:
                    break;
                case ConsoleKey.Oem102:
                    break;
                case ConsoleKey.Process:
                    break;
                case ConsoleKey.Packet:
                    break;
                case ConsoleKey.Attention:
                    break;
                case ConsoleKey.CrSel:
                    break;
                case ConsoleKey.ExSel:
                    break;
                case ConsoleKey.EraseEndOfFile:
                    break;
                case ConsoleKey.Play:
                    break;
                case ConsoleKey.Zoom:
                    break;
                case ConsoleKey.NoName:
                    break;
                case ConsoleKey.Pa1:
                    break;
                case ConsoleKey.OemClear:
                    break;
                default:
                    break;
            }
            return $"{blurb}";
        }

        private static void action_left()
        {
            switch (_current_action_index){
                case 0://claw close
                    send("v");
                    break;
                case 1://wrist counterclockwise
                    send("<");
                    break;
                case 2://elbow back
                    send("w");
                    break;
                case 3://knee back
                    send("t");
                    break; 
                case 4://base back
                    send("d");
                    break;
                case 5://rotate
                    send("l");
                    break;                                                                               
            }
        }

        private static void action_right()
        {
            switch (_current_action_index){
                case 0:
                    //claw open
                    send("^");
                    break;
                case 1:
                    //wrist clockwise
                    send(">");
                    break;
                case 2://elbow forward
                    send("q");
                    break;
                case 3://knee forward
                    send("e");
                    break; 
                case 4://base forward
                    send("u");
                    break;
                case 5://turn clockwise
                    send("k");
                    break; 
            }
        }

        private static void set_current_up()
        {
            _current_action_index--;
            if(_current_action_index < 0){
                _current_action_index = _actionables.Length-1;
            }
            say(_actionables[_current_action_index]);
        }

        private static void set_current_down()
        {
            _current_action_index++;
            if(_current_action_index >= _actionables.Length){
                _current_action_index = 0;
            }
            say(_actionables[_current_action_index]);
        }
    }
}

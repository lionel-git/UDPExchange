using System.Net;
using System.Net.Sockets;
using System.Text;

// Todo: Add name resolver on ip address

namespace UdpReceiver
{
    public class UDPListener
    {
        private int listenPort_;
        private string path_;
        private StreamWriter outputFile_;

        public UDPListener(int port, string path, bool appendLog)
        {
            listenPort_ = port;
            outputFile_ = new StreamWriter(path, appendLog);
            path_ = path;
        }

        public void Start()
        {
            Console.WriteLine($"Starting UDP listener on port {listenPort_} write to file: {path_}");
            var now0 = DateTime.Now.ToUniversalTime();
            outputFile_.WriteLine($"{now0.ToString("yyyy-MM-dd hh:mm:ss.fffffff")} =============================================================");
            outputFile_.Flush();
            UdpClient listener = new UdpClient(listenPort_);
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort_);
            try
            {
                Console.WriteLine("Waiting for message...");
                while (true)
                {
                    byte[] bytes = listener.Receive(ref groupEP);
                    var hostname = DNSResolver.Resolve(groupEP.Address.ToString());
                    var now = DateTime.Now.ToUniversalTime();
                    outputFile_.WriteLine($"{now.ToString("yyyy-MM-dd hh:mm:ss.fffffff")},{hostname},{Encoding.ASCII.GetString(bytes, 0, bytes.Length)}");
                    outputFile_.Flush();
                }

            }
            catch (SocketException e)
            {
                Console.WriteLine(e);
            }
            finally
            {
                Console.WriteLine("Closing");
                outputFile_.Close();
                listener.Close();
            }
        }
    }
}

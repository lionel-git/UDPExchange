using System.Net;
using System.Net.Sockets;
using System.Text;

// Todo: Add name resolver on ip address

namespace UdpReceiver
{
    public class UDPListener
    {
        private int listenPort;

        public UDPListener(int port)
        {
            listenPort = port;
        }

        public void Start()
        {
            Console.WriteLine("Starting UDP listener on port " + listenPort);
            UdpClient listener = new UdpClient(listenPort);
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);
            try
            {
                while (true)
                {
                    Console.WriteLine("Waiting for broadcast");
                    byte[] bytes = listener.Receive(ref groupEP);

                    var hostname = DNSResolver.Resolve(groupEP.Address.ToString());

                    Console.WriteLine($"Received broadcast from {groupEP} ({hostname}):");
                    Console.WriteLine($" {Encoding.ASCII.GetString(bytes, 0, bytes.Length)}");
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine(e);
            }
            finally
            {
                listener.Close();
            }
        }
    }
}

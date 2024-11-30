using System.Net;

namespace UdpReceiver
{
    public class DNSResolver
    {
        public static string Resolve(string ip)
        {
            try
            {
                var hostEntry = Dns.GetHostEntry(ip);
                return hostEntry.HostName;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return ip;
            }
        }
    }
}

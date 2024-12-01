using System.Net;

namespace UdpReceiver
{
    public class DNSResolver
    {
        private static readonly Dictionary<string, string> dnsCache_ = [];

        public static string Resolve(string ip)
        {
            try
            {
                if (dnsCache_.TryGetValue(ip, out string? hostname))
                {
                    return hostname;
                }
                var hostEntry = Dns.GetHostEntry(ip);
                dnsCache_[ip] = hostEntry.HostName;
                return hostEntry.HostName;
            }
            catch (Exception e)
            {
                Console.WriteLine($"Unable to resolve ip: '{ip}'");
                Console.WriteLine(e);
                return ip;
            }
        }
    }
}

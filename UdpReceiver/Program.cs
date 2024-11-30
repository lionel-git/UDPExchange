namespace UdpReceiver
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var listener = new UDPListener(9000);
            listener.Start();
            Console.WriteLine("Press key to stop...");
            Console.ReadKey();
        }
    }
}

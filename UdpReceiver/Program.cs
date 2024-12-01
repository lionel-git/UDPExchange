namespace UdpReceiver
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string outputFilePath = "output.log";
            var listener = new UDPListener(9000, outputFilePath);
            listener.Start();
            Console.WriteLine("Press key to stop...");
            Console.ReadKey();
        }
    }
}

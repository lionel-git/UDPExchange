namespace UdpReceiver
{
    internal class Program
    {
        static void Main(string[] args)
        {
            try
            {
                string outputFilePath = @"output.log";
                var listener = new UDPListener(9000, outputFilePath, true);
                listener.Start();
                Console.WriteLine("Press key to stop...");
                Console.ReadKey();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }
    }
}

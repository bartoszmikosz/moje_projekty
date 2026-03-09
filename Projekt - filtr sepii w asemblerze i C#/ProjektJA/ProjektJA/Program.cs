/*
 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ProjektJA
{
    internal class Program
    {
        [DllImport(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\x64\Debug\JAProjektDll1.dll")]
        static extern int MyProc1();

        [DllImport(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\x64\Debug\JAProjektDll2.dll")]
        static extern int MyProc2();

        [DllImport(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\x64\Debug\JAProjektDllCPlus.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Hello();

        [DllImport(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\x64\Debug\JAProjektDllCPlus.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Add(int a, int b);

        static void Main(string[] args)
        {
            //MyProc1();
            MyProc2();

            Console.WriteLine("Wywołanie funkcji z C DLL:");
            Hello();
            int result = Add(3, 5);
            Console.WriteLine($"3 + 5 = {result}");
        }
    }
}

*/

/*
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

class Program
{
    
    [DllImport(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\x64\Debug\JAProjektDll1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void ModifyPixel(IntPtr pixelData);

    static void Main()
    {
        
        Bitmap bmp = new Bitmap(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\ProjektJA\test2.bmp");
        var rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
        var bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bmp.PixelFormat);
        IntPtr ptr = bmpData.Scan0;
        bmp.UnlockBits(bmpData);
        ModifyPixel(ptr);
        bmp.Save(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\ProjektJA\modified3.bmp");
        Console.WriteLine("Piksel zmodyfikowany!");
    }
}
*/

using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

class Program
{
    [DllImport(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\x64\Debug\JAProjektDll1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void ModifyPixel(IntPtr pixelData, int width);

    [DllImport(@"C:\Users\Bartek Bartosław\source\repos\ProjektJA\x64\Debug\JAProjektDllCPlus.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void ModifyPixel2(IntPtr rowPtr, int width, int bytesPerPixel);

    static void Main()
    {
        string inputPath = @"C:\Users\Bartek Bartosław\source\repos\ProjektJA\ProjektJA\cat.bmp";
        string outputPath = @"C:\Users\Bartek Bartosław\source\repos\ProjektJA\ProjektJA\modified6.bmp";

        using (Bitmap sourceBmp = new Bitmap(inputPath))
        {
            Bitmap bmp = sourceBmp.Clone(new Rectangle(0, 0, sourceBmp.Width, sourceBmp.Height), PixelFormat.Format32bppArgb);
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
            BitmapData bmpData = bmp.LockBits(rect, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);

            IntPtr ptr = bmpData.Scan0;
            int bytesPerPixel = Image.GetPixelFormatSize(bmp.PixelFormat) / 8;
            int stride = bmpData.Stride;
            int height = bmp.Height;

            Console.Write("Podaj liczbę wątków (0 = automatycznie): ");
            int threads = int.Parse(Console.ReadLine() ?? "0");
            if (threads <= 0)
                threads = Environment.ProcessorCount;

            Console.WriteLine($"Używam {threads} wątków...");

            // Podział obrazu na fragmenty pionowe
            int rowsPerThread = height / threads;
            Task[] tasks = new Task[threads];

            var sw = Stopwatch.StartNew();

            for (int t = 0; t < threads; t++)
            {
                int startY = t * rowsPerThread;
                //int endY = (t == threads - 1) ? height : startY + rowsPerThread;
                int endY = startY + rowsPerThread;
                int width = bmp.Width;

                tasks[t] = Task.Run(() =>
                {
                    for (int y = startY; y < endY; y++)
                    {
                        IntPtr rowPtr = ptr + y * stride;
                        // przekazujemy wskaźnik na początek danego wiersza
                        ModifyPixel(rowPtr, width);
                        //ModifyPixel2(rowPtr, width, bytesPerPixel);
                    }
                    if (threads * rowsPerThread + (endY / rowsPerThread) <= height)
                    {
                        int y = threads * rowsPerThread + (endY / rowsPerThread) - 1;
                        IntPtr rowPtr = ptr + y * stride;
                        //ModifyPixel2(rowPtr, width, bytesPerPixel);
                        ModifyPixel(rowPtr, width);
                    }
                });
            }

            Task.WaitAll(tasks);

            sw.Stop();

            bmp.UnlockBits(bmpData);
            bmp.Save(outputPath);
            bmp.Dispose();

            // Console.WriteLine("Przetwarzanie zakończone!");
            Console.WriteLine($"Przetwarzanie zakończone! Czas: {sw.ElapsedMilliseconds} ms");

        }

        
    }
}


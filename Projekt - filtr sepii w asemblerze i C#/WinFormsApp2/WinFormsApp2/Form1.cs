using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;


/*
 * Aplikacja: WinForms Sepia Processor
 * Interfejs u¿ytkownika (C#) s³u¿¹cy do porównania wydajnoœci algorytmu Sepia.
 * Dzia³anie: 
 * - Dynamicznie ³aduje biblioteki DLL (ASM i C++) przy u¿yciu LoadLibrary.
 * - Wykorzystuje mechanizm wielow¹tkowoœci (Task.Run) 
 * do podzia³u obrazu na poziome pasy przetwarzane przez osobne w¹tki.
 * - Komunikuje siê z kodem natywnym poprzez wskaŸniki (IntPtr) i blokowanie 
 * pamiêci obrazu (LockBits)
 */

namespace WinFormsApp2
{

    public partial class Form1 : Form
    {


        // DELEGATY (Wzorce funkcji)
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ModifyPixelDelegate(IntPtr pixelData, int width);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void ModifyPixel2Delegate(IntPtr rowPtr, int width);

        // INSTRANCJE (U¿ywane w kodzie)
        private ModifyPixelDelegate ModifyPixel;
        private ModifyPixel2Delegate ModifyPixel2;

        // IMPORTY SYSTEMOWE (Do ³adowania plików)
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Ansi)]
        private static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

        private Bitmap originalBitmap = null;


        public Form1()
        {
            InitializeComponent();
            LoadNativeDlls();
            // Konfiguracja suwaka w¹tków
            trackThreads.Minimum = 1;
            trackThreads.Maximum = 64;
            trackThreads.Value = Math.Min(Environment.ProcessorCount, 64);
            lblThreadCount.Text = $"W¹tki: {trackThreads.Value}";
            rbAsm.Checked = true;

        }

        private void LoadNativeDlls()
        {
            // Pobiera œcie¿kê do folderu z EXE
            string exePath = AppDomain.CurrentDomain.BaseDirectory;

            // £adowanie bibliotek z folderu aplikacji
            IntPtr pCpp = LoadLibrary(Path.Combine(exePath, "JAProjektDllCPlus.dll"));

            if (pCpp == IntPtr.Zero)
            {
                MessageBox.Show("Nie znaleziono JAProjektDllCPlus.dll w: " + exePath);
                return;
            }
            IntPtr pAsm = LoadLibrary(Path.Combine(exePath, "JAProjektDll1.dll"));
            if (pAsm == IntPtr.Zero)
            {
                MessageBox.Show("Nie znaleziono JAProjektDll1.dll w: " + exePath);
                return;
            }
           

            if (pAsm == IntPtr.Zero || pCpp == IntPtr.Zero)
            {
                MessageBox.Show("B³¹d: Nie znaleziono plików DLL obok programu!");
                return;
            }

            // Podpinanie funkcji pod delegaty
            ModifyPixel = (ModifyPixelDelegate)Marshal.GetDelegateForFunctionPointer(
                GetProcAddress(pAsm, "ModifyPixel"), typeof(ModifyPixelDelegate));

            ModifyPixel2 = (ModifyPixel2Delegate)Marshal.GetDelegateForFunctionPointer(
                GetProcAddress(pCpp, "ModifyPixel2"), typeof(ModifyPixel2Delegate));
        }

        private void rbAsm_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void lblTime_Click(object sender, EventArgs e)
        {

        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.Filter = "Pliki obrazów|*.bmp;*.jpg;*.png";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    originalBitmap = new Bitmap(ofd.FileName);
                    pictureBox.Image = originalBitmap;
                }
            }

        }

        private async void btnRun_Click(object sender, EventArgs e)
        {
            if (originalBitmap == null)
            {
                MessageBox.Show("Najpierw wczytaj obraz!");
                return;
            }

            // Klonowanie do formatu 32bpp ARGB
            Bitmap bmp = originalBitmap.Clone(new Rectangle(0, 0, originalBitmap.Width, originalBitmap.Height), PixelFormat.Format32bppArgb);
            

            int threads = trackThreads.Value;
            bool useAsm = rbAsm.Checked;
            int height = bmp.Height;
            int width = bmp.Width;

            Stopwatch sw = new Stopwatch();

            // Task.Run zapobiega "zamro¿eniu" interfejsu u¿ytkownika
            await Task.Run(() =>
            {
                BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
                IntPtr ptr = bmpData.Scan0;
                int stride = bmpData.Stride;

                int rowsPerThread = height / threads;
                Task[] tasks = new Task[threads];

                sw.Start();
                for (int t = 0; t < threads; t++)
                {
                    int startY = t * rowsPerThread;
                    // Obs³uga reszty wierszy przez ostatni w¹tek
                    int endY = startY + rowsPerThread;

                    tasks[t] = Task.Run(() =>
                    {
                        for (int y = startY; y < endY; y++)
                        {
                            IntPtr rowPtr = ptr + y * stride;
                            if (useAsm)
                                ModifyPixel(rowPtr, width);
                            else
                                ModifyPixel2(rowPtr, width);
                        }
                        if (threads * rowsPerThread + (endY / rowsPerThread) <= height)
                        {
                            int y = threads * rowsPerThread + (endY / rowsPerThread) - 1;
                            IntPtr rowPtr = ptr + y * stride;
                            if(useAsm)
                                ModifyPixel(rowPtr, width);
                            else
                                ModifyPixel2(rowPtr, width);
                            //ModifyPixel2(rowPtr, width, bytesPerPixel);

                        }
                    });
                }
                Task.WaitAll(tasks);
                sw.Stop();
                // string outputPath = @"C:\Users\Bartek Bartos³aw\source\repos\ProjektJA\ProjektJA\modified7.bmp";
                string exePath = AppDomain.CurrentDomain.BaseDirectory;
                string outputPath = Path.Combine(exePath, "modified_result.bmp");
                bmp.Save(outputPath);
                bmp.UnlockBits(bmpData);
            });
            string tempPath = Path.Combine(Path.GetTempPath(), "temp_processed_image.bmp");
            bmp.Save(tempPath, ImageFormat.Bmp);
            bmp.Dispose();
            using (FileStream fs = new FileStream(tempPath, FileMode.Open, FileAccess.Read))
            {
                pictureBox.Image = Image.FromStream(fs);
            }
            // Wyœwietlenie wyniku i czasu
            //pictureBox.Image = bmp;
            //pictureBox.Image = bmp;
            //pictureBox.Invalidate(); // Wymusza ponowne odrysowanie kontrolki
            //pictureBox.Update();
            //Bitmap displayBmp = new Bitmap(bmp); // Tworzy now¹, "czyst¹" kopiê dla UI
            //pictureBox.Image = displayBmp;
            lblTime.Text = $"Czas: {sw.ElapsedMilliseconds} ms";

        }

        private void trackThreads_Scroll(object sender, EventArgs e)
        {
            lblThreadCount.Text = $"W¹tki: {trackThreads.Value}";
        }

        private void rbCpp_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}

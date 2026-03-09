namespace WinFormsApp2
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            pictureBox = new PictureBox();
            btnLoad = new Button();
            btnRun = new Button();
            trackThreads = new TrackBar();
            lblThreadCount = new Label();
            lblTime = new Label();
            rbAsm = new RadioButton();
            rbCpp = new RadioButton();
            ((System.ComponentModel.ISupportInitialize)pictureBox).BeginInit();
            ((System.ComponentModel.ISupportInitialize)trackThreads).BeginInit();
            SuspendLayout();
            // 
            // pictureBox
            // 
            pictureBox.Location = new Point(113, 25);
            pictureBox.Name = "pictureBox";
            pictureBox.Size = new Size(638, 339);
            pictureBox.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox.TabIndex = 0;
            pictureBox.TabStop = false;
            // 
            // btnLoad
            // 
            btnLoad.Location = new Point(140, 394);
            btnLoad.Name = "btnLoad";
            btnLoad.Size = new Size(134, 29);
            btnLoad.TabIndex = 1;
            btnLoad.Text = "Wczytaj Obraz";
            btnLoad.UseVisualStyleBackColor = true;
            btnLoad.Click += btnLoad_Click;
            // 
            // btnRun
            // 
            btnRun.Location = new Point(531, 394);
            btnRun.Name = "btnRun";
            btnRun.Size = new Size(94, 29);
            btnRun.TabIndex = 2;
            btnRun.Text = "Uruchom";
            btnRun.UseVisualStyleBackColor = true;
            btnRun.Click += btnRun_Click;
            // 
            // trackThreads
            // 
            trackThreads.Location = new Point(122, 493);
            trackThreads.Name = "trackThreads";
            trackThreads.Size = new Size(130, 56);
            trackThreads.TabIndex = 3;
            trackThreads.Scroll += trackThreads_Scroll;
            // 
            // lblThreadCount
            // 
            lblThreadCount.AutoSize = true;
            lblThreadCount.Location = new Point(138, 450);
            lblThreadCount.Name = "lblThreadCount";
            lblThreadCount.Size = new Size(61, 20);
            lblThreadCount.TabIndex = 4;
            lblThreadCount.Text = "Wątki: 0";
            // 
            // lblTime
            // 
            lblTime.AutoSize = true;
            lblTime.Location = new Point(665, 403);
            lblTime.Name = "lblTime";
            lblTime.Size = new Size(73, 20);
            lblTime.TabIndex = 5;
            lblTime.Text = "Czas: 0ms";
            lblTime.Click += lblTime_Click;
            // 
            // rbAsm
            // 
            rbAsm.AutoSize = true;
            rbAsm.Location = new Point(588, 461);
            rbAsm.Name = "rbAsm";
            rbAsm.Size = new Size(61, 24);
            rbAsm.TabIndex = 6;
            rbAsm.TabStop = true;
            rbAsm.Text = "ASM";
            rbAsm.UseVisualStyleBackColor = true;
            rbAsm.CheckedChanged += rbAsm_CheckedChanged;
            // 
            // rbCpp
            // 
            rbCpp.AutoSize = true;
            rbCpp.Location = new Point(588, 493);
            rbCpp.Name = "rbCpp";
            rbCpp.Size = new Size(59, 24);
            rbCpp.TabIndex = 7;
            rbCpp.TabStop = true;
            rbCpp.Text = "C++";
            rbCpp.UseVisualStyleBackColor = true;
            rbCpp.CheckedChanged += rbCpp_CheckedChanged;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(866, 631);
            Controls.Add(rbCpp);
            Controls.Add(rbAsm);
            Controls.Add(lblTime);
            Controls.Add(lblThreadCount);
            Controls.Add(trackThreads);
            Controls.Add(btnRun);
            Controls.Add(btnLoad);
            Controls.Add(pictureBox);
            Name = "Form1";
            Text = "Edytor Sepii";
            ((System.ComponentModel.ISupportInitialize)pictureBox).EndInit();
            ((System.ComponentModel.ISupportInitialize)trackThreads).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private PictureBox pictureBox;
        private Button btnLoad;
        private Button btnRun;
        private TrackBar trackThreads;
        private Label lblThreadCount;
        private Label lblTime;
        private RadioButton rbAsm;
        private RadioButton rbCpp;
    }
}

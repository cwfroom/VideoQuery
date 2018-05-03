#pragma once
#include "Data.h"
namespace VideoQuery {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  dataVideoPlayButton;
	protected:

	private: System::Windows::Forms::PictureBox^  dataVideoImage;
	private: System::Windows::Forms::PictureBox^  queryVideoImage;
	private: System::Windows::Forms::Button^  queryVideoPlayButton;
	private: System::Windows::Forms::ListBox^  dataVideoListBox;
	private: System::Windows::Forms::Label^  dataVideoLabel;
	private: System::Windows::Forms::Label^  queryVideoLabel;
	private: System::Windows::Forms::TrackBar^  queryVideoTrackBar;
	private: System::Windows::Forms::TrackBar^  dataVideoTrackBar;
	private: System::Windows::Forms::Button^  dataVideoLoadButton;
	private: System::Windows::Forms::PictureBox^  perFrameBox;

	private: System::Windows::Forms::TextBox^  queryVideoNameText;
	private: System::Windows::Forms::Button^  queryVideoLoadButton;
	private: System::Windows::Forms::Button^  playBothButton;
	private: System::Windows::Forms::CheckBox^  underscoreCheck;
	private: System::Windows::Forms::Label^  perFrameLabel;
	private: System::Windows::Forms::Label^  colorAccuracyLabel;
	private: System::Windows::Forms::PictureBox^  colorAccuracyBox;
	private: System::Windows::Forms::Label^  motionAccuracyLabel;
	private: System::Windows::Forms::PictureBox^  motionAccuracyBox;
	private: System::Windows::Forms::Label^  audioAccuracyLabel;
	private: System::Windows::Forms::PictureBox^  audioAccuracyBox;












	protected:


	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dataVideoPlayButton = (gcnew System::Windows::Forms::Button());
			this->dataVideoImage = (gcnew System::Windows::Forms::PictureBox());
			this->queryVideoImage = (gcnew System::Windows::Forms::PictureBox());
			this->queryVideoPlayButton = (gcnew System::Windows::Forms::Button());
			this->dataVideoListBox = (gcnew System::Windows::Forms::ListBox());
			this->dataVideoLabel = (gcnew System::Windows::Forms::Label());
			this->queryVideoLabel = (gcnew System::Windows::Forms::Label());
			this->queryVideoTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->dataVideoTrackBar = (gcnew System::Windows::Forms::TrackBar());
			this->dataVideoLoadButton = (gcnew System::Windows::Forms::Button());
			this->perFrameBox = (gcnew System::Windows::Forms::PictureBox());
			this->queryVideoNameText = (gcnew System::Windows::Forms::TextBox());
			this->queryVideoLoadButton = (gcnew System::Windows::Forms::Button());
			this->playBothButton = (gcnew System::Windows::Forms::Button());
			this->underscoreCheck = (gcnew System::Windows::Forms::CheckBox());
			this->perFrameLabel = (gcnew System::Windows::Forms::Label());
			this->colorAccuracyLabel = (gcnew System::Windows::Forms::Label());
			this->colorAccuracyBox = (gcnew System::Windows::Forms::PictureBox());
			this->motionAccuracyLabel = (gcnew System::Windows::Forms::Label());
			this->motionAccuracyBox = (gcnew System::Windows::Forms::PictureBox());
			this->audioAccuracyLabel = (gcnew System::Windows::Forms::Label());
			this->audioAccuracyBox = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoImage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->queryVideoImage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->queryVideoTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoTrackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->perFrameBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->colorAccuracyBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->motionAccuracyBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->audioAccuracyBox))->BeginInit();
			this->SuspendLayout();
			// 
			// dataVideoPlayButton
			// 
			this->dataVideoPlayButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->dataVideoPlayButton->Location = System::Drawing::Point(583, 390);
			this->dataVideoPlayButton->Name = L"dataVideoPlayButton";
			this->dataVideoPlayButton->Size = System::Drawing::Size(75, 32);
			this->dataVideoPlayButton->TabIndex = 0;
			this->dataVideoPlayButton->Text = L"Play";
			this->dataVideoPlayButton->UseVisualStyleBackColor = true;
			this->dataVideoPlayButton->Click += gcnew System::EventHandler(this, &MainWindow::dataVideoPlayButton_Click);
			// 
			// dataVideoImage
			// 
			this->dataVideoImage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dataVideoImage->Location = System::Drawing::Point(444, 45);
			this->dataVideoImage->Name = L"dataVideoImage";
			this->dataVideoImage->Size = System::Drawing::Size(352, 288);
			this->dataVideoImage->TabIndex = 1;
			this->dataVideoImage->TabStop = false;
			// 
			// queryVideoImage
			// 
			this->queryVideoImage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->queryVideoImage->Location = System::Drawing::Point(37, 45);
			this->queryVideoImage->Name = L"queryVideoImage";
			this->queryVideoImage->Size = System::Drawing::Size(352, 288);
			this->queryVideoImage->TabIndex = 2;
			this->queryVideoImage->TabStop = false;
			// 
			// queryVideoPlayButton
			// 
			this->queryVideoPlayButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->queryVideoPlayButton->Location = System::Drawing::Point(151, 390);
			this->queryVideoPlayButton->Name = L"queryVideoPlayButton";
			this->queryVideoPlayButton->Size = System::Drawing::Size(75, 32);
			this->queryVideoPlayButton->TabIndex = 3;
			this->queryVideoPlayButton->Text = L"Play";
			this->queryVideoPlayButton->UseVisualStyleBackColor = true;
			this->queryVideoPlayButton->Click += gcnew System::EventHandler(this, &MainWindow::queryVideoPlayButton_Click);
			// 
			// dataVideoListBox
			// 
			this->dataVideoListBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->dataVideoListBox->FormattingEnabled = true;
			this->dataVideoListBox->ItemHeight = 20;
			this->dataVideoListBox->Location = System::Drawing::Point(444, 441);
			this->dataVideoListBox->Name = L"dataVideoListBox";
			this->dataVideoListBox->Size = System::Drawing::Size(352, 184);
			this->dataVideoListBox->TabIndex = 4;
			// 
			// dataVideoLabel
			// 
			this->dataVideoLabel->AutoSize = true;
			this->dataVideoLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->dataVideoLabel->Location = System::Drawing::Point(440, 22);
			this->dataVideoLabel->Name = L"dataVideoLabel";
			this->dataVideoLabel->Size = System::Drawing::Size(122, 20);
			this->dataVideoLabel->TabIndex = 5;
			this->dataVideoLabel->Text = L"No video loaded";
			// 
			// queryVideoLabel
			// 
			this->queryVideoLabel->AutoSize = true;
			this->queryVideoLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->queryVideoLabel->Location = System::Drawing::Point(33, 22);
			this->queryVideoLabel->Name = L"queryVideoLabel";
			this->queryVideoLabel->Size = System::Drawing::Size(122, 20);
			this->queryVideoLabel->TabIndex = 6;
			this->queryVideoLabel->Text = L"No video loaded";
			// 
			// queryVideoTrackBar
			// 
			this->queryVideoTrackBar->Location = System::Drawing::Point(37, 339);
			this->queryVideoTrackBar->Name = L"queryVideoTrackBar";
			this->queryVideoTrackBar->Size = System::Drawing::Size(352, 45);
			this->queryVideoTrackBar->TabIndex = 7;
			this->queryVideoTrackBar->Scroll += gcnew System::EventHandler(this, &MainWindow::queryVideoTrackBar_Scroll);
			// 
			// dataVideoTrackBar
			// 
			this->dataVideoTrackBar->Location = System::Drawing::Point(444, 339);
			this->dataVideoTrackBar->Name = L"dataVideoTrackBar";
			this->dataVideoTrackBar->Size = System::Drawing::Size(352, 45);
			this->dataVideoTrackBar->TabIndex = 8;
			this->dataVideoTrackBar->Scroll += gcnew System::EventHandler(this, &MainWindow::dataVideoTrackBar_Scroll);
			// 
			// dataVideoLoadButton
			// 
			this->dataVideoLoadButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->dataVideoLoadButton->Location = System::Drawing::Point(583, 637);
			this->dataVideoLoadButton->Name = L"dataVideoLoadButton";
			this->dataVideoLoadButton->Size = System::Drawing::Size(75, 32);
			this->dataVideoLoadButton->TabIndex = 9;
			this->dataVideoLoadButton->Text = L"Load";
			this->dataVideoLoadButton->UseVisualStyleBackColor = true;
			this->dataVideoLoadButton->Click += gcnew System::EventHandler(this, &MainWindow::dataVideoLoadButton_Click);
			// 
			// perFrameBox
			// 
			this->perFrameBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->perFrameBox->Location = System::Drawing::Point(893, 45);
			this->perFrameBox->Name = L"perFrameBox";
			this->perFrameBox->Size = System::Drawing::Size(300, 100);
			this->perFrameBox->TabIndex = 10;
			this->perFrameBox->TabStop = false;
			this->perFrameBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::perFrameBox_Paint);
			// 
			// queryVideoNameText
			// 
			this->queryVideoNameText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->queryVideoNameText->Location = System::Drawing::Point(37, 441);
			this->queryVideoNameText->Name = L"queryVideoNameText";
			this->queryVideoNameText->Size = System::Drawing::Size(352, 26);
			this->queryVideoNameText->TabIndex = 11;
			this->queryVideoNameText->Text = L"first";
			// 
			// queryVideoLoadButton
			// 
			this->queryVideoLoadButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->queryVideoLoadButton->Location = System::Drawing::Point(151, 521);
			this->queryVideoLoadButton->Name = L"queryVideoLoadButton";
			this->queryVideoLoadButton->Size = System::Drawing::Size(84, 32);
			this->queryVideoLoadButton->TabIndex = 12;
			this->queryVideoLoadButton->Text = L"Query";
			this->queryVideoLoadButton->UseVisualStyleBackColor = true;
			this->queryVideoLoadButton->Click += gcnew System::EventHandler(this, &MainWindow::queryVideoLoadButton_Click);
			// 
			// playBothButton
			// 
			this->playBothButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->playBothButton->Location = System::Drawing::Point(365, 390);
			this->playBothButton->Name = L"playBothButton";
			this->playBothButton->Size = System::Drawing::Size(105, 32);
			this->playBothButton->TabIndex = 14;
			this->playBothButton->Text = L"Play Both";
			this->playBothButton->UseVisualStyleBackColor = true;
			this->playBothButton->Click += gcnew System::EventHandler(this, &MainWindow::playBothButton_Click);
			// 
			// underscoreCheck
			// 
			this->underscoreCheck->AutoSize = true;
			this->underscoreCheck->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->underscoreCheck->Location = System::Drawing::Point(37, 481);
			this->underscoreCheck->Name = L"underscoreCheck";
			this->underscoreCheck->Size = System::Drawing::Size(195, 24);
			this->underscoreCheck->TabIndex = 15;
			this->underscoreCheck->Text = L"Underscore in file name";
			this->underscoreCheck->UseVisualStyleBackColor = true;
			// 
			// perFrameLabel
			// 
			this->perFrameLabel->AutoSize = true;
			this->perFrameLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->perFrameLabel->Location = System::Drawing::Point(889, 22);
			this->perFrameLabel->Name = L"perFrameLabel";
			this->perFrameLabel->Size = System::Drawing::Size(83, 20);
			this->perFrameLabel->TabIndex = 16;
			this->perFrameLabel->Text = L"Per Frame";
			// 
			// colorAccuracyLabel
			// 
			this->colorAccuracyLabel->AutoSize = true;
			this->colorAccuracyLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->colorAccuracyLabel->Location = System::Drawing::Point(889, 178);
			this->colorAccuracyLabel->Name = L"colorAccuracyLabel";
			this->colorAccuracyLabel->Size = System::Drawing::Size(115, 20);
			this->colorAccuracyLabel->TabIndex = 17;
			this->colorAccuracyLabel->Text = L"Color Accuracy";
			// 
			// colorAccuracyBox
			// 
			this->colorAccuracyBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->colorAccuracyBox->Location = System::Drawing::Point(893, 201);
			this->colorAccuracyBox->Name = L"colorAccuracyBox";
			this->colorAccuracyBox->Size = System::Drawing::Size(300, 100);
			this->colorAccuracyBox->TabIndex = 18;
			this->colorAccuracyBox->TabStop = false;
			this->colorAccuracyBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::colorAccuracyBox_Paint);
			// 
			// motionAccuracyLabel
			// 
			this->motionAccuracyLabel->AutoSize = true;
			this->motionAccuracyLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->motionAccuracyLabel->Location = System::Drawing::Point(889, 329);
			this->motionAccuracyLabel->Name = L"motionAccuracyLabel";
			this->motionAccuracyLabel->Size = System::Drawing::Size(126, 20);
			this->motionAccuracyLabel->TabIndex = 19;
			this->motionAccuracyLabel->Text = L"Motion Accuracy";
			// 
			// motionAccuracyBox
			// 
			this->motionAccuracyBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->motionAccuracyBox->Location = System::Drawing::Point(893, 352);
			this->motionAccuracyBox->Name = L"motionAccuracyBox";
			this->motionAccuracyBox->Size = System::Drawing::Size(300, 100);
			this->motionAccuracyBox->TabIndex = 20;
			this->motionAccuracyBox->TabStop = false;
			this->motionAccuracyBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::motionAccuracyBox_Paint);
			// 
			// audioAccuracyLabel
			// 
			this->audioAccuracyLabel->AutoSize = true;
			this->audioAccuracyLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->audioAccuracyLabel->Location = System::Drawing::Point(889, 481);
			this->audioAccuracyLabel->Name = L"audioAccuracyLabel";
			this->audioAccuracyLabel->Size = System::Drawing::Size(119, 20);
			this->audioAccuracyLabel->TabIndex = 21;
			this->audioAccuracyLabel->Text = L"Audio Accuracy";
			// 
			// audioAccuracyBox
			// 
			this->audioAccuracyBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->audioAccuracyBox->Location = System::Drawing::Point(893, 504);
			this->audioAccuracyBox->Name = L"audioAccuracyBox";
			this->audioAccuracyBox->Size = System::Drawing::Size(300, 100);
			this->audioAccuracyBox->TabIndex = 22;
			this->audioAccuracyBox->TabStop = false;
			this->audioAccuracyBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::audioAccuracyBox_Paint);
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1264, 681);
			this->Controls->Add(this->audioAccuracyBox);
			this->Controls->Add(this->audioAccuracyLabel);
			this->Controls->Add(this->motionAccuracyBox);
			this->Controls->Add(this->motionAccuracyLabel);
			this->Controls->Add(this->colorAccuracyBox);
			this->Controls->Add(this->colorAccuracyLabel);
			this->Controls->Add(this->perFrameLabel);
			this->Controls->Add(this->underscoreCheck);
			this->Controls->Add(this->playBothButton);
			this->Controls->Add(this->queryVideoLoadButton);
			this->Controls->Add(this->queryVideoNameText);
			this->Controls->Add(this->perFrameBox);
			this->Controls->Add(this->dataVideoLoadButton);
			this->Controls->Add(this->dataVideoTrackBar);
			this->Controls->Add(this->queryVideoTrackBar);
			this->Controls->Add(this->queryVideoLabel);
			this->Controls->Add(this->dataVideoLabel);
			this->Controls->Add(this->dataVideoListBox);
			this->Controls->Add(this->queryVideoPlayButton);
			this->Controls->Add(this->queryVideoImage);
			this->Controls->Add(this->dataVideoImage);
			this->Controls->Add(this->dataVideoPlayButton);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(1280, 720);
			this->MinimumSize = System::Drawing::Size(1280, 720);
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoImage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->queryVideoImage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->queryVideoTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoTrackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->perFrameBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->colorAccuracyBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->motionAccuracyBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->audioAccuracyBox))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		Data data;
		System::Void setMetricsBox(int* arr, int size);
		System::Void paintHelper(Graphics^ g, Eigen::VectorXf vec);
		System::Void perFrameBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		System::Void colorAccuracyBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		System::Void motionAccuracyBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		System::Void audioAccuracyBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e);

	private:
		System::Void dataVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void queryVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void dataVideoLoadButton_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void dataVideoTrackBar_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void queryVideoTrackBar_Scroll(System::Object^  sender, System::EventArgs^  e);
		System::Void queryVideoLoadButton_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void playBothButton_Click(System::Object^  sender, System::EventArgs^  e);
};

}

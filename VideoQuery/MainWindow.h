#pragma once

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
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

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
	private: System::Windows::Forms::Button^  videoPlayButton;
	private: System::Windows::Forms::PictureBox^  dataVideoImage;



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
			this->videoPlayButton = (gcnew System::Windows::Forms::Button());
			this->dataVideoImage = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoImage))->BeginInit();
			this->SuspendLayout();
			// 
			// videoPlayButton
			// 
			this->videoPlayButton->Location = System::Drawing::Point(546, 526);
			this->videoPlayButton->Name = L"videoPlayButton";
			this->videoPlayButton->Size = System::Drawing::Size(75, 23);
			this->videoPlayButton->TabIndex = 0;
			this->videoPlayButton->Text = L"Play";
			this->videoPlayButton->UseVisualStyleBackColor = true;
			this->videoPlayButton->Click += gcnew System::EventHandler(this, &MainWindow::videoPlayButton_Click);
			// 
			// dataVideoImage
			// 
			this->dataVideoImage->Location = System::Drawing::Point(400, 212);
			this->dataVideoImage->Name = L"dataVideoImage";
			this->dataVideoImage->Size = System::Drawing::Size(352, 288);
			this->dataVideoImage->TabIndex = 1;
			this->dataVideoImage->TabStop = false;
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 561);
			this->Controls->Add(this->dataVideoImage);
			this->Controls->Add(this->videoPlayButton);
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoImage))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
		System::Void videoPlayButton_Click(System::Object^  sender, System::EventArgs^  e);
	
	};
}

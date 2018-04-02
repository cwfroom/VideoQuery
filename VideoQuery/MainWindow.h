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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoImage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->queryVideoImage))->BeginInit();
			this->SuspendLayout();
			// 
			// dataVideoPlayButton
			// 
			this->dataVideoPlayButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->dataVideoPlayButton->Location = System::Drawing::Point(425, 517);
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
			this->dataVideoImage->Location = System::Drawing::Point(407, 212);
			this->dataVideoImage->Name = L"dataVideoImage";
			this->dataVideoImage->Size = System::Drawing::Size(352, 288);
			this->dataVideoImage->TabIndex = 1;
			this->dataVideoImage->TabStop = false;
			// 
			// queryVideoImage
			// 
			this->queryVideoImage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->queryVideoImage->Location = System::Drawing::Point(22, 212);
			this->queryVideoImage->Name = L"queryVideoImage";
			this->queryVideoImage->Size = System::Drawing::Size(352, 288);
			this->queryVideoImage->TabIndex = 2;
			this->queryVideoImage->TabStop = false;
			// 
			// queryVideoPlayButton
			// 
			this->queryVideoPlayButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->queryVideoPlayButton->Location = System::Drawing::Point(33, 517);
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
			this->dataVideoListBox->Location = System::Drawing::Point(407, 12);
			this->dataVideoListBox->Name = L"dataVideoListBox";
			this->dataVideoListBox->Size = System::Drawing::Size(352, 164);
			this->dataVideoListBox->TabIndex = 4;
			// 
			// dataVideoLabel
			// 
			this->dataVideoLabel->AutoSize = true;
			this->dataVideoLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->dataVideoLabel->Location = System::Drawing::Point(412, 222);
			this->dataVideoLabel->Name = L"dataVideoLabel";
			this->dataVideoLabel->Size = System::Drawing::Size(70, 20);
			this->dataVideoLabel->TabIndex = 5;
			this->dataVideoLabel->Text = L"Loading ";
			this->dataVideoLabel->Visible = false;
			// 
			// queryVideoLabel
			// 
			this->queryVideoLabel->AutoSize = true;
			this->queryVideoLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->queryVideoLabel->Location = System::Drawing::Point(29, 222);
			this->queryVideoLabel->Name = L"queryVideoLabel";
			this->queryVideoLabel->Size = System::Drawing::Size(70, 20);
			this->queryVideoLabel->TabIndex = 6;
			this->queryVideoLabel->Text = L"Loading ";
			this->queryVideoLabel->Visible = false;
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 561);
			this->Controls->Add(this->queryVideoLabel);
			this->Controls->Add(this->dataVideoLabel);
			this->Controls->Add(this->dataVideoListBox);
			this->Controls->Add(this->queryVideoPlayButton);
			this->Controls->Add(this->queryVideoImage);
			this->Controls->Add(this->dataVideoImage);
			this->Controls->Add(this->dataVideoPlayButton);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(800, 600);
			this->MinimumSize = System::Drawing::Size(800, 600);
			this->Name = L"MainWindow";
			this->Text = L"MainWindow";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataVideoImage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->queryVideoImage))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		Data data;

	private:
		System::Void dataVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void queryVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e);
};
}

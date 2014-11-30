#using <System.dll>
#using <System.Windows.Forms.dll>   

#include <string>
#include <iostream>
#include <cstdio>
#include <msclr\marshal_cppstd.h>
using namespace msclr::interop;
using namespace std;

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

struct Record
{
	Record()
	{
		next = NULL;
	}
	Record(int id, string f_name, string s_name, float value)
	{
		number = id;
		name = f_name;
		surname = s_name;
		mark = value;
		next = NULL;
	}

	string name, surname;
	float mark;
	int number;

	Record* next_pointer()
	{
		return next;
	}
	void new_next(Record *pointer)
	{
		next = pointer;
	}	

	Record *next;	
};
struct MyList
{
	MyList()
	{		
		initiated = false;		
		init();
	}

	string name()
	{
		return current->name;
	}
	string surname()
	{
		return current->surname;
	}
	int number()
	{
		return current->number;
	}
	float mark()
	{
		return current->mark;
	}

	void add(Record *rec)
	{		
		if (initiated)
		{
			end->new_next(rec);
			end = rec;
		}
		else
		{
			begin = rec;
			current = rec;
			end = rec;
			initiated = true;
		}
	}
	void print(FILE *out)
	{
		Record *rec = begin;
		do
		{
			fprintf(out, "%i %s %s %.4f\n",
				rec->number,
				rec->name.c_str(),
				rec->surname.c_str(),
				rec->mark);

			rec = rec->next_pointer();
		} while (rec != NULL);
	}
	int find_record(int id)
	{
		Record *seek = begin;
		while (seek != end)
		{
			if (seek->number == id) break;
			seek = seek->next_pointer();
		}

		if (seek->number == id) 
		{
			current = seek;
			return 0;
		}
		else
		{
			return -1;
		}
	}
	void destroy_all()
	{
		if (!initiated) return;
		initiated = false;
		
		Record *tmp1 = begin;
		do
		{
			Record *tmp2 = tmp1;			
			tmp1 = tmp2->next_pointer();			
			delete tmp2;			
		} while (tmp1 != NULL);

		init();
	}
	void init()
	{
		begin = end = current = new Record();
	}

	Record *current, *begin, *end;	
	bool initiated;
};
MyList list;
public ref class MyForm : public System::Windows::Forms::Form
{
public:
	MyForm(void)
	{
		InitializeComponent();		
		in = NULL; out = NULL;		
	}
protected:
	~MyForm()
	{
		if (components)
		{
			delete components;
		}
		if (in != NULL)
			fclose(in); 
		if (out != NULL)
			fclose(out);
	}
private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::ListBox^  listBox1;
private: System::Windows::Forms::TextBox^  textBox1;
private: System::Windows::Forms::TextBox^  textBox2;
private: System::Windows::Forms::TextBox^  textBox3;
private: System::Windows::Forms::Button^  button2;
private: System::Windows::Forms::Button^  button3;
private: FILE *in;
private: FILE *out;
protected: 

private:
	System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
	void InitializeComponent(void)
	{
		this->button1 = (gcnew System::Windows::Forms::Button());
		this->listBox1 = (gcnew System::Windows::Forms::ListBox());
		this->textBox1 = (gcnew System::Windows::Forms::TextBox());
		this->textBox2 = (gcnew System::Windows::Forms::TextBox());
		this->textBox3 = (gcnew System::Windows::Forms::TextBox());
		this->button2 = (gcnew System::Windows::Forms::Button());
		this->button3 = (gcnew System::Windows::Forms::Button());
		this->SuspendLayout();
		// 
		// button1
		// 
		this->button1->Location = System::Drawing::Point(59, 12);
		this->button1->Name = L"button1";
		this->button1->Size = System::Drawing::Size(75, 23);
		this->button1->TabIndex = 0;
		this->button1->Text = L"Add";
		this->button1->UseVisualStyleBackColor = true;
		this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
		// 
		// listBox1
		// 
		this->listBox1->FormattingEnabled = true;
		this->listBox1->Location = System::Drawing::Point(12, 12);
		this->listBox1->Name = L"listBox1";
		this->listBox1->Size = System::Drawing::Size(41, 212);
		this->listBox1->TabIndex = 1;
		this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::listBox1_SelectedIndexChanged);
		// 
		// textBox1
		// 
		this->textBox1->Location = System::Drawing::Point(59, 41);
		this->textBox1->Name = L"textBox1";
		this->textBox1->Text = L"Example";
		this->textBox1->Size = System::Drawing::Size(100, 20);
		this->textBox1->TabIndex = 2;
		// 
		// textBox2
		// 
		this->textBox2->Location = System::Drawing::Point(59, 68);
		this->textBox2->Name = L"textBox2";
		this->textBox2->Text = L"Prime";
		this->textBox2->Size = System::Drawing::Size(100, 20);
		this->textBox2->TabIndex = 3;
		// 
		// textBox3
		// 
		this->textBox3->Location = System::Drawing::Point(59, 95);
		this->textBox3->Name = L"textBox3";
		this->textBox3->Text = L"0,1234";
		this->textBox3->Size = System::Drawing::Size(100, 20);
		this->textBox3->TabIndex = 4;
		// 
		// button2
		// 
		this->button2->Location = System::Drawing::Point(59, 201);
		this->button2->Name = L"button2";
		this->button2->Size = System::Drawing::Size(75, 23);
		this->button2->TabIndex = 5;
		this->button2->Text = L"Save";
		this->button2->UseVisualStyleBackColor = true;
		this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
		// 
		// button3
		// 
		this->button3->Location = System::Drawing::Point(59, 172);
		this->button3->Name = L"button3";
		this->button3->Size = System::Drawing::Size(75, 23);
		this->button3->TabIndex = 6;
		this->button3->Text = L"Read";
		this->button3->UseVisualStyleBackColor = true;
		this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
		// 
		// MyForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(174, 236);
		this->Controls->Add(this->button3);
		this->Controls->Add(this->button2);
		this->Controls->Add(this->textBox3);
		this->Controls->Add(this->textBox2);
		this->Controls->Add(this->textBox1);
		this->Controls->Add(this->listBox1);
		this->Controls->Add(this->button1);
		this->Name = L"MyForm";
		this->Text = L"data";
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion
private: string sysstr2str(String ^str)
		 {
			 string str1 = "";
			 for (int i = 0; i < str->Length; i++)
				 str1 += (char)str[i];
			 return str1;
		 } 
private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 // Get the currently selected item in the ListBox.
			 String ^sss = listBox1->SelectedItem->ToString();
			 int id = Convert::ToInt32(sss, 10);

			 list.find_record(id);
			 String ^name = gcnew String(list.name().c_str());
			 String ^surname = gcnew String(list.surname().c_str());
			 String ^mark = gcnew String(list.mark().ToString());				
			 textBox1->Text = name;
			 textBox2->Text = surname;
			 textBox3->Text = mark;

			 //// Find the string in ListBox2.
			 //int index = listBox1->FindString(sss);
			 //// If the item was not found in ListBox 2 display a message box,
			 ////  otherwise select it in ListBox2.
			 //if(index == -1)
			 //   MessageBox::Show("Item is not available in ListBox2");
			 //else
			 //   listBox1->SetSelected(index,true);
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {						
			 int hm = listBox1->Items->Count;
			 string aa = sysstr2str(textBox1->Text);
			 string bb = sysstr2str(textBox2->Text);
			 double ff = Convert::ToDouble(textBox3->Text->ToString());

			 listBox1->Items->Add(hm);
			 list.add( new Record(
				 hm, aa, bb, ff)
				 );
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {				 
			 in = fopen("group.txt","r");
			 listBox1->Items->Clear();
			 list.destroy_all();
			 char tmp1[128], tmp2[128];
			 int id;
			 while (fscanf(in,"%i",&id) == 1)
			 {
				 string name, surname; float mark;
				 fscanf(in,"%s%s%f",&tmp1,&tmp2,&mark);
				 name = tmp1;
				 surname = tmp2;						
				 list.add( new Record(
					 id,name,surname,mark));
				 listBox1->Items->Add(id);
			 }
			 fclose(in);
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 out = fopen("group.txt","w");
			 list.print(out);
			 fclose(out);
		 }
};


int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm());
}
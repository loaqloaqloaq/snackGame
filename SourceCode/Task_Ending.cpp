//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"
#include "randomLib.h"
#include "sound.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->logoPosY = 270;
		this->frameCnt = 0;

		this->endFont = DG::Font::Create("�l�r �S�V�b�N", 20, 30);
		this->alpha[0] = 0;
		this->alpha[1] = 0;
		this->alpha[2] = 0;

		this->alphaAdding = true;

		se::LoadFile("show", "./data/sound/maou_se_battle14.wav");
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		if (inp.ST.down) {
			if (this->frameCnt < 45 + 30) {
				this->frameCnt = 45 + 30 - 1;
				this->alpha[0] = 1.0f;
			}
			else if(this->frameCnt < 45 + 90) {
				this->frameCnt = 45 + 90 - 1;
				this->alpha[1] = 1.0f;
			}
			else if (this->frameCnt < 45 + 150) {
				this->frameCnt = 45 + 150 - 1;
				this->alpha[2] = 1.0f;
			}
			else {
				ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
				ge->CreateEffect(99, ML::Vec2(0, 0));
				se::Play("ok");
			}
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		if (inp.B1.down) {
			ML::Vec2 p;
			ge->CreateEffect(0,p);
		}
		if (inp.B2.down) {
			ML::Vec2 p((const float)GetRandom(0, 300), (const float)GetRandom(0, 300));
			ge->CreateEffect((int)GetRandom(0,8), p);

		}

		this->frameCnt++;
		float showSpeed = 0.03f;
		//fade effect 45 frame
		if (this->frameCnt > 45 + 30) {
			if (this->alpha[0] < 1.0f) this->alpha[0] += showSpeed;
		}
		if (this->frameCnt > 45 + 90) {
			if (this->alpha[1] < 1.0f) this->alpha[1] += showSpeed;
		}
		if (this->frameCnt > 45 + 150) {
			if (this->alphaAdding) {
				if (this->alpha[2] < 1.0f) this->alpha[2] += showSpeed;
				else this->alphaAdding = false;
			}
			else {
				if (this->alpha[2] > 0) this->alpha[2] -= showSpeed;
				else this->alphaAdding = true;
			}			
		}

		//play sound fx
		if (this->frameCnt == 45 + 30 || this->frameCnt == 45 + 90 || this->frameCnt == 45 + 150) {
			se::Play("show");
		}
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{	
		//ending text 1
		this->DrawText("���Ȃ��̏W�߂���񂲂�", 0);		

		//ending text 2
		this->DrawText(to_string(ge->score)+" ��!", 1);

		//ending text 3		
		this->DrawText("PRESS START KEY", 2);
		
	}
	void Object::DrawText(string msg, int index) {
		int posY[] = {
			150,
			250,
			450
		};
		ML::Box2D endFontDraw((msg.length() * 20 / 2) * -1, -(30 / 2), msg.length() * 20, 30);
		endFontDraw.Offset((int)ge->screen2DWidth / 2, posY[index]);
		this->endFont->Draw(endFontDraw,
			msg,
			ML::Color(this->alpha[index], 1, 1, 1)
		);

	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}
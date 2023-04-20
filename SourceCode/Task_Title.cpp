//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include "randomLib.h"
#include  "Task_Effect00.h"
#include "sound.h"

#include "Task_Game.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->TitleLogo01 = DG::Image::Create("./data/image/title_text.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->TitleLogo01.reset();
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
		this->controller = ge->in1;
		this->render2D_Priority[1] = 0.5f;
		int RenderTime = 0;

		ge->stage = 1;


		//�f�o�b�O�p�t�H���g�̏���
		this->startFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);

		//���y�t�@�C���̓ǂݍ���
		//BGM
		// BGM�̗��p����t�@�C���͋Ȃ̒����ɂ���ăT�C�Y���ς��܂��B
		// ��ʓI��Wav�͒��g���g�`�f�[�^�ƌĂ΂��T�C�Y���傫�ȃf�[�^�ɂ�܂��B
		// ���mp3�̓l�b�g�Ȃǂł̈�����z�肵�����k�`���ł��B
		// BGM��p����ꍇ��mp3�`���̃t�@�C���𗘗p���܂��傤�B
		// ���Ȃ݂ɂ��̃T���v���̃t�@�C����
		// �Đ�����1�F30�قڂ̋Ȃ�
		// mp3 = 4.3MB wav = 19MB�ł��B
		bgm::LoadFile("bgm1", "./data/sound/bgm/�e�ރL���`_2.mp3");
		bgm::Play("bgm1");
		//se
		// se�͌��ʉ��ł��B
		// bgm�Ƃ̍ő�̈Ⴂ�͂ЂƂ̉��y�t�@�C���̓����Đ��ɑΉ����Ă��邱�Ƃł��B
		// se��wav�t�@�C�������������Ƃ��o���܂���Bmp3�̓G���[�ɂȂ�܂��B
		// �����Đ��̕K�v���Ȃ����̂�BGM
		// �����Đ����K�v�Ȃ��̂�se�Ǝg�������Ă��������B
	  // �܂����̃T���v���̂悤�ȓ��{��t�@�C�����͂�߂������ǂ��ł��B
		// �ǂݍ��݃G���[�̌��ɂȂ�܂��B		
		se::LoadFile("ok", "./data/sound/ok.wav");		

		startFontAlp = 0.0f;
		alpAdd = true;
		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		bgm::Stop("bgm1");
		if (!ge->QuitFlag() && this->nextTaskCreate) {			
			Game::Object::Create(true);
		}
		startFont.reset();

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		int input = 0;
		//							16�i		2�i	
		if (inp.B1.down) input |= 0x0001;	// 0000000001
		if (inp.B2.down) input |= 0x0002;	// 0000000010
		if (inp.B3.down) input |= 0x0004;	// 0000000100
		if (inp.B4.down) input |= 0x0008;	// 0000001000
		if (inp.ST.down) input |= 0x0100;	// 0100000000

		if (input & 0x0100) {
			se::Play("ok");
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(98, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		if (input & 0x0002) {
			se::Play("se1");
		}
		if (input & 0x0004) {
			se::Play("se2");
		}

		//�uPRESS START KEY�v����
		float sparkSpeed = 0.03f;
		if (alpAdd) {
			if (startFontAlp < 1.0f) startFontAlp += sparkSpeed;
			else {
				startFontAlp = 1.0f;
				alpAdd = false;
			}
		}
		else {
			if (startFontAlp > 0.0f) startFontAlp -= sparkSpeed;
			else {
				startFontAlp = 0.0f;
				alpAdd = true;
			}
		}

		return;

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{		
		ML::Box2D logoDraw(-605/2, -124, 605, 88), logoSrc(0, 0, 605, 88);
		logoDraw.Offset((int)ge->screen2DWidth/2, (int)ge->screen2DHeight/2);
		this->res->TitleLogo01->Draw(logoDraw, logoSrc);

		string startText = "PRESS START KEY";
		ML::Box2D startFontDraw( (startText.length() * 30 / 2) * -1, 100, startText.length() * 30, 30);
		startFontDraw.Offset((int)ge->screen2DWidth / 2, (int)ge->screen2DHeight / 2);
		this->startFont->Draw(startFontDraw,
			startText,
			ML::Color(startFontAlp,1,1,1)
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
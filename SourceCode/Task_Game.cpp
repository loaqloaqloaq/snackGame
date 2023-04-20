//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"

#include "randomLib.h"

#include  "Task_Ending.h"
#include "Task_Player.h"
#include "Task_Map.h"
#include "Task_Apple.h"
#include "sound.h"

namespace  Game
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
		srand((unsigned)time(NULL));
		ge->status = ge->Status::Normal;
		ge->score = 0;
		//�f�o�b�O�p�t�H���g�̏���
		this->TestFont = DG::Font::Create("�l�r �S�V�b�N", 30, 30);
		this->stKeyFont = DG::Font::Create("�l�r �S�V�b�N", 15, 25);
		//���^�X�N�̐���
		auto  pl = Player::Object::Create(true);		
		pl->pos = ML::Vec2((int)ge->screen2DWidth / 2, (int)ge->screen2DHeight / 2);

		
		ge->appleCnt[0] = 0;
		ge->appleCnt[1] = 0;
		ge->appleMaxCnt = 1;		

		this->alpha = 1.0f;
		this->alphaAdding = false;

		bgm::LoadFile("bgm2", "./data/sound/bgm/maou_bgm_ethnic25.mp3");
		bgm::LoadFile("sad bgm", "./data/sound/bgm/maou_bgm_ethnic30.mp3");
		bgm::Play("bgm2");		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���	
		bgm::AllStop();
		ge->KillAll_G("�{��");
		ge->KillAll_G("�v���C���[");		
		ge->KillAll_G("�����S");	
		this->TestFont.reset();
		this->stKeyFont.reset();
		ge->appleCnt[0] = 0;
		ge->appleCnt[1] = 0;
		ge->appleMaxCnt = 0;
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (ge->status == ge->Status::End) {
			bgm::Stop("bgm2");			
			bgm::Play("sad bgm");
		}
		auto inp = ge->in1->GetState( );
		if (inp.ST.down && ge->status == ge->Status::End) {
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(99, ML::Vec2(0, 0));
			se::Play("ok");
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		ge->appleMaxCnt = max(1, 1 + ge->score / 5);
		for (ge->appleCnt; ge->appleCnt[0]+ ge->appleCnt[1] < ge->appleMaxCnt;) {
			auto  apple = Apple::Object::Create(true);
			apple->type = ge->appleCnt[0] == 0 ? 0 : rand() % 2;
			ge->appleCnt[apple->type]++;


			int margin = 32;
			ML::Vec2 tmp = ML::Vec2(margin/2 + rand() % (ge->screen2DWidth - apple->hitBase.w - margin), margin/2 + rand() % (ge->screen2DHeight - apple->hitBase.h - margin));
			auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
			while (apple->hitBase.OffsetCopy(tmp).Hit(pl->hitBase.OffsetCopy(pl->pos))) {
				tmp= ML::Vec2(margin + rand() % (ge->screen2DWidth - apple->hitBase.w - margin), margin + rand() % (ge->screen2DHeight - apple->hitBase.h - margin));
			}

			apple->pos = tmp;
		}
		if (this->alphaAdding) {
			if (this->alpha < 1.0f) this->alpha += 0.03f;
			else this->alphaAdding = false;
		}
		else {
			if (this->alpha > 0.0f) this->alpha -= 0.03f;
			else this->alphaAdding = true;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		this->TestFont->Draw(ML::Box2D(50, 50, ge->screen2DWidth, ge->screen2DHeight),
			"Score:" + to_string(ge->score)
		);

		if (ge->status == ge->Status::End) {
			string gameOver = "GAME OVER";
			string stKey = "PRESS START KEY";

			ML::Box2D gameOverTextBox((gameOver.length() * 30 / 2) * -1, -(30 / 2), gameOver.length() * 30, 30);
			gameOverTextBox.Offset((int)ge->screen2DWidth / 2, 250);
			this->TestFont->Draw(gameOverTextBox, gameOver);

			ML::Box2D stKeyTextBox((stKey.length() * 15 / 2) * -1, -(25 / 2), stKey.length() * 15, 25);
			stKeyTextBox.Offset((int)ge->screen2DWidth / 2, 350);
			this->stKeyFont->Draw(stKeyTextBox, stKey, ML::Color(this->alpha, 1, 1, 1));
		}
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
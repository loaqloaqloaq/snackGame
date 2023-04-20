//-------------------------------------------------------------------
//ゲーム本編
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		srand((unsigned)time(NULL));
		ge->status = ge->Status::Normal;
		ge->score = 0;
		//デバッグ用フォントの準備
		this->TestFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);
		this->stKeyFont = DG::Font::Create("ＭＳ ゴシック", 15, 25);
		//★タスクの生成
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
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放	
		bgm::AllStop();
		ge->KillAll_G("本編");
		ge->KillAll_G("プレイヤー");		
		ge->KillAll_G("リンゴ");	
		this->TestFont.reset();
		this->stKeyFont.reset();
		ge->appleCnt[0] = 0;
		ge->appleCnt[1] = 0;
		ge->appleMaxCnt = 0;
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (ge->status == ge->Status::End) {
			bgm::Stop("bgm2");			
			bgm::Play("sad bgm");
		}
		auto inp = ge->in1->GetState( );
		if (inp.ST.down && ge->status == ge->Status::End) {
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
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
	//「２Ｄ描画」１フレーム毎に行う処理
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

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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
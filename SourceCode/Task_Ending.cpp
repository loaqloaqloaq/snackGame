//-------------------------------------------------------------------
//エンディング
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
		this->logoPosY = 270;
		this->frameCnt = 0;

		this->endFont = DG::Font::Create("ＭＳ ゴシック", 20, 30);
		this->alpha[0] = 0;
		this->alpha[1] = 0;
		this->alpha[2] = 0;

		this->alphaAdding = true;

		se::LoadFile("show", "./data/sound/maou_se_battle14.wav");
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
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
				ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{	
		//ending text 1
		this->DrawText("あなたの集めたりんごは", 0);		

		//ending text 2
		this->DrawText(to_string(ge->score)+" 個!", 1);

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
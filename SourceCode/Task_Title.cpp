//-------------------------------------------------------------------
//タイトル画面
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->TitleLogo01 = DG::Image::Create("./data/image/title_text.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->TitleLogo01.reset();
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
		this->controller = ge->in1;
		this->render2D_Priority[1] = 0.5f;
		int RenderTime = 0;

		ge->stage = 1;


		//デバッグ用フォントの準備
		this->startFont = DG::Font::Create("ＭＳ ゴシック", 30, 30);

		//音楽ファイルの読み込み
		//BGM
		// BGMの利用するファイルは曲の長さによってサイズが変わります。
		// 一般的にWavは中身が波形データと呼ばれるサイズが大きなデータにります。
		// 一方mp3はネットなどでの扱いを想定した圧縮形式です。
		// BGMを用いる場合はmp3形式のファイルを利用しましょう。
		// ちなみにこのサンプルのファイルは
		// 再生時間1：30ほぼの曲で
		// mp3 = 4.3MB wav = 19MBです。
		bgm::LoadFile("bgm1", "./data/sound/bgm/弾むキモチ_2.mp3");
		bgm::Play("bgm1");
		//se
		// seは効果音です。
		// bgmとの最大の違いはひとつの音楽ファイルの同時再生に対応していることです。
		// seはwavファイルしか扱うことが出来ません。mp3はエラーになります。
		// 同時再生の必要がないものはBGM
		// 同時再生が必要なものはseと使い分けてください。
	  // またこのサンプルのような日本語ファイル名はやめた方が良いです。
		// 読み込みエラーの元になります。		
		se::LoadFile("ok", "./data/sound/ok.wav");		

		startFontAlp = 0.0f;
		alpAdd = true;
		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		bgm::Stop("bgm1");
		if (!ge->QuitFlag() && this->nextTaskCreate) {			
			Game::Object::Create(true);
		}
		startFont.reset();

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		int input = 0;
		//							16進		2進	
		if (inp.B1.down) input |= 0x0001;	// 0000000001
		if (inp.B2.down) input |= 0x0002;	// 0000000010
		if (inp.B3.down) input |= 0x0004;	// 0000000100
		if (inp.B4.down) input |= 0x0008;	// 0000001000
		if (inp.ST.down) input |= 0x0100;	// 0100000000

		if (input & 0x0100) {
			se::Play("ok");
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
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

		//「PRESS START KEY」明滅
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
	//「２Ｄ描画」１フレーム毎に行う処理
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
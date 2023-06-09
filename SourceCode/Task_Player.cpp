//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include "sound.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XÌú»
	bool  Resource::Initialize()
	{
		this->playerImg = DG::Image::Create("./data/image/chara.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XÌðú
	bool  Resource::Finalize()
	{
		this->playerImg.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uú»v^XN¶¬ÉPñ¾¯s¤
	bool  Object::Initialize()
	{
		//X[p[NXú»
		__super::Initialize(defGroupName, defName, true);
		//\[XNX¶¬or\[X¤L
		this->res = Resource::Create();
		
		//f[^ú»		
		this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(-64/2, -64/2, 64, 64);
		//0¶@1ã@2E@3º
		this->angle = 3;
		this->speed = 2.0f;

		se::LoadFile("damage", "./data/sound/maou_se_battle16.wav");
		//^XNÌ¶¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uI¹v^XNÁÅÉPñ¾¯s¤
	bool  Object::Finalize()
	{
		//f[^^XNðú


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ø«p¬^XNÌ¶¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[És¤
	void  Object::UpDate()
	{
		if (ge->status == ge->Status::Normal) {			
			this->moveCnt++;
			//üÍ»è
			auto inp = ge->in1->GetState();
			if (inp.LStick.BL.down) { this->angle = 0; }
			else if (inp.LStick.BU.down) { this->angle = 1; }
			else if (inp.LStick.BR.down) { this->angle = 2; }
			else if (inp.LStick.BD.down) { this->angle = 3; }
			//Ú®
			//0¶@1ã@2E@3º
			switch (this->angle) {
			case 0: //¶
				this->moveVec = ML::Vec2(-this->speed, 0);
				break;
			case 1: //ã
				this->moveVec = ML::Vec2(0, -this->speed);
				break;
			case 2: //E
				this->moveVec = ML::Vec2(this->speed, 0);
				break;
			case 3: //º
				this->moveVec = ML::Vec2(0, this->speed);
				break;
			}

			while (this->moveVec.x != 0) {
				if (this->moveVec.x >= 1) {
					this->pos.x++;
					this->moveVec.x--;
				}
				else if (this->moveVec.x <= 1) {
					this->pos.x--;
					this->moveVec.x++;
				}
				else {
					this->pos.x += this->moveVec.x;
					this->moveVec.x = 0.0f;
				}
				if (this->CheckOutOfMap()) {
					ge->status = ge->Status::End;
				}
			}
			while (this->moveVec.y != 0) {
				if (this->moveVec.y >= 1) {
					this->pos.y++;
					this->moveVec.y--;
				}
				else if (this->moveVec.y <= 1) {
					this->pos.y--;
					this->moveVec.y++;
				}
				else {
					this->pos.y += this->moveVec.y;
					this->moveVec.y = 0.0f;
				}
				if (this->CheckOutOfMap()) {
					ge->status = ge->Status::End;
				}
			}
		}
		
	}
	bool Object::CheckOutOfMap() {
		ML::Box2D player = this->hitBase.OffsetCopy(this->pos);
		if (
			player.x<0 ||
			player.y<0 ||
			player.x + player.w>(int)ge->screen2DWidth ||
			player.y + player.h>(int)ge->screen2DHeight
		){
			se::Play("damage");
			return true;
		}
		else return false;
	}
	//-------------------------------------------------------------------
	//uQc`ævPt[És¤
	void  Object::Render2D_AF()
	{
		//0¶@1ã@2E@3º
		int playerSrcY[4] = {
			64 * 1,
			64 * 3,
			64 * 2,
			64 * 0
		};		
		int animSpeed = max(1, (int)(15.0f - this->speed));
		ML::Box2D playerSrc(moveCnt / animSpeed % 3 * 64, playerSrcY[this->angle], 64, 64), playerDraw;
		playerDraw=this->hitBase.OffsetCopy(this->pos);
		this->res->playerImg->Draw(playerDraw, playerSrc);
	}

	//
	//ÈºÍî{IÉÏXsvÈ\bh
	//
	//-------------------------------------------------------------------
	//^XN¶¬û
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWÉo^

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYÉ¸sµ½çKill
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
	//\[XNXÌ¶¬
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
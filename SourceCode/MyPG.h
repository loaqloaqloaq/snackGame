#pragma once
#include "GameEngine_Ver3_83.h"
#include <fstream>
#include <sstream>

//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------

#include "fpscounter.h"

#include "task_Effect00.h"

//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//�J������{�`
	class Camera
	{
		Camera() { }
		Camera(
			const ML::Vec3& tg_,	//	��ʑ̂̈ʒu
			const ML::Vec3& pos_,	//	�J�����̈ʒu
			const ML::Vec3& up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
			float				fov_,	//	����p
			float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
			float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
			float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j			
	public:
		//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;			//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
	//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
	//	�s����
		ML::Mat4x4  matView, matProj;
		~Camera();
		using SP = shared_ptr<Camera>;
		//	�J�����𐶐�����
		static SP Create(
			const ML::Vec3& tg_,	//	��ʑ̂̈ʒu
			const ML::Vec3& pos_,	//	�J�����̈ʒu
			const ML::Vec3& up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
			float				fov_,	//	����p
			float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
			float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
			float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
//	�J�����̐ݒ�
		void UpDate();
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine();
		//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize(HWND wnd_);
		~MyGameEngine();
		//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate();

		//3DPG1�Ή��ɂ��ǉ�
			//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);

		//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
		//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	�J����

		XI::Mouse::SP  mouse;
		XI::GamePad::SP  in1, in2, in3, in4;	//��荇�����S�{
		//�Q�c�J������`
		ML::Box2D				camera2D;	//  �Q�c�X�N���[������p
		map<string, float> evFlags;

		bool GameOverFlag = false; //�Q�[���I�[�o�[�t���O
		bool GameClearFlag = false;//�N���A�t���O
		bool unHitEnemy = false;//�G�Ƃ̓����蔻��
		int GameOverCnt;
		int MaxGameOver;

		bool			Jump2Check;		//�W�����v2����
		bool			DestroyFlag; //�j��t���O
		bool			AttackFlag; //�U���t���O

		bool			TransparentFlag; //�����t���O


		//----------------------------------------------
		FPSCounter* c;

		int stage;

		void CreateEffect(int no, ML::Vec2 pos);
		void Dbg_ToConsole(const char* str, ...);
		void Dbg_ToDisplay(int x, int y, const char* str, ...);
		void Dbg_ToTitle(const char* str, ...);
		void Dbg_ToBox(const char* str, ...);
		void Dbg_ToFileOut(const char* str, ...);

		//�f�o�b�O��`
		//��������
		DG::Image::SP rectImage;

		enum DEBUGRECTMODE {
			RED, BLUE, GREEN, ORANGE,
			REDFRAME, BLUEFRAME, GREENFRAME, ORANGEFRAME,
		};
		struct DebugRectInfo {
			ML::Box2D	r;
			int			flag;
		};

		std::vector<DebugRectInfo> debugRectArray;
		//�������p
		void debugRectLoad();
		void debugRectReset();
		void debugRectDraw();
		//�\����������`�ƕ\�����[�h��n��
		//RED,BLUE,GREEN,ORANGE�̋�`��FRAME�͘g�̂�
		void debugRect(ML::Box2D r_, int flag = 4, int offsetx = 0, int offsety = 0);

		enum COUNTER_FLAGS {
			NONE,
			ACTIVE,//�ғ���
			LIMIT,//�I������
			END,//�I��
		};

		struct COUNTER {
			COUNTER_FLAGS flag;
			int count;
		};
		std::map < std::string, COUNTER > counterMap;


		void InitCounter();
		void ActCounter();
		void AllEndCounter();
		COUNTER_FLAGS getCounterFlag(string str);
		void StartCounter(string str, int count);

		enum Status {
			Normal, End
		};

		Status status;

		int score;

		int appleMaxCnt,appleCnt[2];
	};
}
extern MyPG::MyGameEngine* ge;



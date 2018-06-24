#ifndef _READER_BASE_HPP_
#define _READER_BASE_HPP_
#include "../NodeBase.hpp"
#include <pipeline/FFL_PipelineEventCallback.hpp>

namespace reader {
	class ReaderStreamManager;
	class ReaderBase : public player::NodeBase {
	public:
		ReaderBase();
		~ReaderBase();
	protected:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//    player::NodeBase
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//  �ɹ�������node
		//
		virtual void onCreate();
		//
		//  �ɹ�ɾ����node
		//
		virtual void onDestroy();
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//    ReaderInterface
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//  ��
		//
		virtual void open(const char* url) ;
		//
		//  ��ͣ�������ǲ��ر�
		//
		virtual void pause() ;
		//
		//  �ָ���
		//
		virtual void resume() ;
		//
		// ���ÿ�ʼ��λ��
		//
		virtual void seek(int64_t pos) ;
		//
		// �ر�
		//
		virtual void close() ;
		//
		// ��ȡ����ʱ��us
		//
		virtual int64_t getDuration() = 0;
		//
		// ��ȡ��ǰ�Ĳ���λ�� us
		//
		virtual int64_t getCurrentPosition() = 0;
	public:
		//
		//  ������������,��open�ɹ��󣬶�ȡ��������������Ϣ��Ȼ���֪ͨIStreamManager
		//
		void setStreamManager(ReaderStreamManager* streamMgr) {
			if (streamMgr) {
				mStreamMgr = streamMgr;
			}
		}
		//
		//  ��ȡ������,���᷵��null��
		//
		ReaderStreamManager* getStreamManager() const{
			return mStreamMgr;
		}
	private:
		ReaderStreamManager* mStreamMgr;
	public:
		//
		// �Ƿ���ͣ״̬
		//
		bool isPaused() const;
	protected:
		player::InputInterface mInput;
		bool initLooper();
		void pauseLooper();
	protected:
		friend class FFL::ClassMethodVoidInputHandler<ReaderBase>;
		//
		// ��ȡ��ѭ��
		//
		void onReadOnce();
		//
		//  ��ȡһ֡����
		//
		virtual void onReadFrame()=0;
	protected:
		friend class FFL::ClassMethodCallback<ReaderBase>;
		FFL::CMutex mRequestLock;
		//
		// open����������ʵ��
		//
		volatile bool mEventOpenPending;
		FFL::String mUrl;
		FFL::sp<FFL::PipelineEvent> mEventOpen;
		void onOpenStub(const FFL::sp<FFL::PipelineEvent>& event);
		virtual void onOpen(const char* url)=0;

		//
		// close����������ʵ��
		//
		volatile bool mEventClosePending;
		FFL::sp<FFL::PipelineEvent> mEventClose;
		void onCloseStub(const FFL::sp<FFL::PipelineEvent>& event);
		virtual void onClose()=0;

		//
		// pause������resume ����ʵ��
		//
		volatile bool mEventPausePending;
		volatile bool mEventResumePending;
		volatile bool mIsPaused;
		FFL::CMutex mPauseLock;
		FFL::CCondition mPauseCond;
		FFL::sp<FFL::PipelineEvent> mEventPause;
		void onPauseStub(const FFL::sp<FFL::PipelineEvent>& event);
		virtual void onPause()=0;
		virtual void onResume()=0;

		//
		// seek����������ʵ��
		//
		volatile bool mEventSeekPending;		
		FFL::sp<FFL::PipelineEvent> mEventSeek;
		void onSeekStub(const FFL::sp<FFL::PipelineEvent>& event);
		virtual void onSeek(int64_t pos)=0;		
	};
}
#endif

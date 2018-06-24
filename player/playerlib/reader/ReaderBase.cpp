#include "ReaderBase.hpp"
#include "../PlayerCore.hpp"

namespace reader {
	class DefaultStreamMgr : public ReaderStreamManager {
	public:
		//
		//  ���reader�µ�һ��stream�������У����ص�ǰ���stream��id
		//
		virtual int32_t  addStream(ReaderBase* reader, StreamPtr stream) {
			return -1;
		}
		//
		//  ���reader�µ�����stream�������У����سɹ�����˼�����
		//  
		//
		virtual uint32_t addStreamVec(ReaderBase* reader, FFL::Vector < StreamPtr > streamVec) {
			return 0;
		}
		//
		//  ������id��ȡһ����ʵ��
		//
		virtual StreamPtr getStream(int32_t id) {
			return NULL;
		}
		//
		//  ��ȡ���reader�µ�������
		//
		virtual void getStreamVec(ReaderBase* reader, FFL::Vector < StreamPtr >& streamVec) {
			streamVec.clear();
		}
	};
	static DefaultStreamMgr gDefaultStreamMgr;

	ReaderBase::ReaderBase():
		mStreamMgr(&gDefaultStreamMgr),
		mEventOpenPending(false),
		mEventClosePending(false),
		mEventPausePending(false),
		mEventResumePending(false),
		mIsPaused(false),
		mEventSeekPending(false){
		mUrl = "";

		mEventOpen = new FFL::PipelineEvent(
			new  FFL::ClassMethodCallback<ReaderBase>(this,&ReaderBase::onOpenStub));
		mEventClose = new FFL::PipelineEvent(
			new  FFL::ClassMethodCallback<ReaderBase>(this, &ReaderBase::onCloseStub));
		mEventPause = new FFL::PipelineEvent(
			new  FFL::ClassMethodCallback<ReaderBase>(this, &ReaderBase::onPauseStub));
		mEventSeek = new FFL::PipelineEvent(
			new  FFL::ClassMethodCallback<ReaderBase>(this, &ReaderBase::onSeekStub));

	}
	ReaderBase::~ReaderBase() {

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//    player::NodeBase
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  �ɹ�������node
	//
	void ReaderBase::onCreate() {
		initLooper();
	}
	//
	//  �ɹ�ɾ����node
	//
	void ReaderBase::onDestroy() {

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//    ReaderInterface
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  ��
	//
	void ReaderBase::open(const char* url){		
		FFL::CMutex::Autolock l(&mRequestLock);
		if (mEventOpenPending) {
			return;
		}

		mUrl = url;
		mEventOpenPending = true;
		postEvent(mEventOpen);
	}
	//
	//  ��ͣ�������ǲ��ر�
	//
	void ReaderBase::pause(){
		FFL::CMutex::Autolock l(&mRequestLock);
		if (mEventPausePending && mIsPaused) {
			return;
		}
		mEventPausePending = true;
		mEventPause->setParams(0, 0);
		postEvent(mEventPause);
	}
	//
	//  �ָ���
	//
	void ReaderBase::resume(){
		FFL::CMutex::Autolock l(&mRequestLock);
		if (mEventResumePending || !mIsPaused) {
			return;
		}
		mEventResumePending = true;		
		mEventPause->setParams(1, 0);
		postEvent(mEventPause);
		mPauseCond.signal();
	}
	//
	// ���ÿ�ʼ��λ��
	//
	void ReaderBase::seek(int64_t pos){
		FFL::CMutex::Autolock l(&mRequestLock);
		if (mEventSeekPending) {
			return;
		}

		mEventSeekPending = true;
		mEventSeek->setParams(pos, 0);		
		postEvent(mEventSeek);		
	}
	//
	// �ر�
	//
	void ReaderBase::close(){
		FFL::CMutex::Autolock l(&mRequestLock);
		if (mEventClosePending) {
			return;
		}
		mEventClosePending = true;
		mPauseCond.signal();
	}
	//
	// �Ƿ���ͣ״̬
	//
	bool ReaderBase::isPaused() const {
		return mIsPaused;
	}
	bool  ReaderBase::initLooper(){
		FFL::sp<FFL::PipelineInputHandler> callback =
			new FFL::ClassMethodVoidInputHandler<ReaderBase>(this, &ReaderBase::onReadOnce);
		player::InputInterface input = createInputInterface(callback, "reader-stream");
		if (!input.isValid()) {
			FFL_LOG_ERROR("invalid input id");
			return false;
		}

		mInput = input;
		FFL::sp<FFL::PipelineSourceConnector> conn = 0;
		if (1) {
			conn = new FFL::PipelineIdleSourceConnector();
		}
		else {
			conn = new FFL::PipelineTimerSourceConnector(3000);
		}		
		getOwner()->getPipeline()->connectSource(input.mNodeId, input.mId, conn);
		return true;
	}
	void ReaderBase::pauseLooper() {
		//
		//  �رյ�ǰ������
		//
		FFL::sp<FFL::PipelineInput> input = getInput(mInput.mId);
		if (!input.isEmpty()) {
			input->requestShutdown();
		}
	}
	//
	// ��ȡ��ѭ��
	//
	void ReaderBase::onReadOnce(){
		if (mIsPaused) {
			mPauseCond.waitRelative(mPauseLock, 100);
			return;
		}		
		onReadFrame();
	}
	void ReaderBase::onOpenStub(const FFL::sp<FFL::PipelineEvent>& event) {
		onOpen(mUrl.c_str());
		{
			FFL::CMutex::Autolock l(&mRequestLock);
			mEventOpenPending = false;
		}
	}	
	//
	// close����������ʵ��
	//
	void ReaderBase::onCloseStub(const FFL::sp<FFL::PipelineEvent>& event) {
		onClose();
		{
			FFL::CMutex::Autolock l(&mRequestLock);
			mEventClosePending = false;
		}
	}


	//
	// pause������resume ����ʵ��
	//
	void ReaderBase::onPauseStub(const FFL::sp<FFL::PipelineEvent>& event) {
		if (event->getParam1() == 0) {
			onPause();
			{
				FFL::CMutex::Autolock l(&mRequestLock);
				mIsPaused = true;
				mEventPausePending = false;				
			}
		}
		else {
			onResume();
			{
				FFL::CMutex::Autolock l(&mRequestLock);
				mIsPaused = false;
				mEventResumePending = false;
			}
		}
	}
	//
	// seek����������ʵ��
	//
	void ReaderBase::onSeekStub(const FFL::sp<FFL::PipelineEvent>& event) {
		onSeek(event->getParam1());
		{
			FFL::CMutex::Autolock l(&mRequestLock);
			mEventSeekPending = false;
		}
	}
}
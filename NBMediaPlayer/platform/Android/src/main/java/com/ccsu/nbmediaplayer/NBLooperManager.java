package com.ccsu.nbmediaplayer;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import java.util.HashMap;
import java.util.Map;

public class NBLooperManager {
    private static final String TAG = NBLooperManager.class.getSimpleName();

    public static class LooperEventRunnable implements Runnable {

        protected NBLooperManager mTDLooperManager;
        protected long mEventQueuePtr;
        protected int mEventId;
        protected long mTimeUs;

        public LooperEventRunnable(NBLooperManager looperManager, long eventQueuePtr, int eventId, long timeUs) {
            mTDLooperManager = looperManager;
            mEventQueuePtr = eventQueuePtr;
            mEventId = eventId;
            mTimeUs = timeUs;
        }

        @Override
        public void run() {
            if (mTDLooperManager != null) {
                mTDLooperManager.runEvent(mEventQueuePtr, mEventId, mTimeUs);
            }
        }
    }

    private Handler mEventHandler = null;
    private Map<Integer, LooperEventRunnable> mRunnableMap = null;
    protected Looper mLooper;

    public NBLooperManager() {
        mRunnableMap = new HashMap<>();
    }

    public void prepare() {
        Looper.prepare();

        mLooper = Looper.myLooper();

        mEventHandler = new Handler(mLooper);
    }

    public void loop() {
        mLooper.loop();
    }

    public void quit() {
        mLooper.quitSafely();
    }

    public void queueRunnableAtTime(long eventQueuePtr, int eventId, long timeUs) {

//        Log.i(TAG, "queueRunnableAtTime : " + eventQueuePtr + " eventId : " + eventId + " timeUs : " + timeUs);

        LooperEventRunnable eventRunnable = new LooperEventRunnable(this, eventQueuePtr, eventId, timeUs);
        mRunnableMap.put(eventId, eventRunnable);
        // convert us second to ms second
//        mEventHandler.postAtTime(eventRunnable, timeUs / 1000);
        mEventHandler.postDelayed(eventRunnable, timeUs / 1000);
    }

    public void removeRunnableWithId(int eventId) {
        LooperEventRunnable eventRunnable = mRunnableMap.get(eventId);
        if (eventRunnable != null) {
            mEventHandler.removeCallbacks(eventRunnable);
            mRunnableMap.remove(eventId);
        }
    }

    public void runEvent(long eventQueuePtr, int eventId, long timeUs) {
        nativeRunEvent(eventQueuePtr, eventId, timeUs);
    }

    private native void nativeRunEvent(long eventQueuePtr, int eventId, long timeUs);
}

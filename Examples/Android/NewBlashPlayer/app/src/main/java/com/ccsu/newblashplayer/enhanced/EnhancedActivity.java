package com.ccsu.newblashplayer.enhanced;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.Button;

import com.ccsu.nbmediaplayer.NBAVPlayer;
import com.ccsu.nbmediaplayer.view.NBEnhancedGLView;
import com.ccsu.newblashplayer.R;
import com.ccsu.newblashplayer.enhanced.helpers.ShaderHelper;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class EnhancedActivity extends AppCompatActivity implements NBEnhancedGLView.Renderer,
                                                                    NBEnhancedGLView.SurfaceTextureListener,
                                                                    View.OnClickListener,
                                                                    NBAVPlayer.OnPreparedListener {

    private static final String TAG = EnhancedActivity.class.getSimpleName();

    private static final String vertexShaderCode =
                    "attribute vec4 vPosition;" +
                    "attribute vec2 vTexcoord;" +
                    "varying vec2 texcoord;" +
                    "void main() {" +
                    "  gl_Position = vPosition;" +
                    "  texcoord = vTexcoord;" +
                    "}";

    private static final String fragmentShaderCode =
                    "precision mediump float;" +
                    "uniform sampler2D texture;" +
                    "varying vec2 texcoord;" +
                    "void main() {" +
                    "  gl_FragColor = texture2D(texture, texcoord);" +
                    "}";

    private static final String extFragmentShaderCode =
            "#extension GL_OES_EGL_image_external : require\n" +
                    "precision mediump float;" +
                    "uniform samplerExternalOES texture;" +
                    "varying vec2 texcoord;" +
                    "void main () {" +
                    "    vec4 color = texture2D(texture, texcoord);" +
                    "    gl_FragColor = color;" +
                    "}";

    // vertex Buffer
    private FloatBuffer vertexBuffer;

    static float squareCoords[] = {
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   // 右上
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f,   // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // 左下

            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // 左下
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  // 左上
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   // 右上
    };

    private NBAVPlayer mMediaPlayer = null;
    private NBEnhancedGLView mEnhancedGLView = null;
    private Button mBackButton = null;

    private static final int BYTES_PER_FLOAT = 4;
    private int mVertCount = -1;

    private int mProgram;
    private int mPositionHandler;
//    private int mVbo[] = new int[1];
    private int mTexName = -1;
    private boolean mIsExternal = false;
    private boolean mIsInited = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_enhanced);

        mEnhancedGLView = this.findViewById(R.id.enhance_surface_view);
        mEnhancedGLView.setEGLContextClientVersion(2);
        mEnhancedGLView.setRenderer(this);
        mEnhancedGLView.setSurfaceTextureListener(this);

        mBackButton = this.findViewById(R.id.back_button);
        mBackButton.setOnClickListener(this);
    }

    @Override
    public void onPrepared(NBAVPlayer paramMediaPlayer) {
        mMediaPlayer.start();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i(TAG, "onSurfaceCreated");

        // start play video
        mMediaPlayer = new NBAVPlayer(this);
        mMediaPlayer.setOnPreparedListener(this);
        try {
            mMediaPlayer.setDataSource("/mnt/sdcard/Movies/test_movie.mp4");
        } catch (IOException e) {
            e.printStackTrace();
        }

        mEnhancedGLView.setPlayer(mMediaPlayer);
//        mMediaPlayer.setSurface(surfaceHolder.getSurface());
        mMediaPlayer.prepareAsync();

//        mTexName = loadTexture(EnhancedActivity.this, R.mipmap.ic_launcher);
//        mIsExternal = false;
    }

    private static Bitmap getBitmap(Context context,int vectorDrawableId) {
        Bitmap bitmap=null;
        if (Build.VERSION.SDK_INT>Build.VERSION_CODES.LOLLIPOP){
            Drawable vectorDrawable = context.getDrawable(vectorDrawableId);
            bitmap = Bitmap.createBitmap(vectorDrawable.getIntrinsicWidth(),
                    vectorDrawable.getIntrinsicHeight(), Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(bitmap);
            vectorDrawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
            vectorDrawable.draw(canvas);
        }else {
            bitmap = BitmapFactory.decodeResource(context.getResources(), vectorDrawableId);
        }
        return bitmap;
    }

    public static int loadTexture(final Context context, final int resourceId)
    {
        final int[] textureHandle = new int[1];

        GLES20.glGenTextures(1, textureHandle, 0);

        if (textureHandle[0] != 0)
        {
//            final BitmapFactory.Options options = new BitmapFactory.Options();
//            options.inScaled = false;	// No pre-scaling

            // Read in the resource
            final Bitmap bitmap = getBitmap(context, resourceId);

            // Bind to the texture in OpenGL
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);

            // Set filtering
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);

            // Load the bitmap into the bound texture.
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);

            // Recycle the bitmap, since its data has been loaded into OpenGL.
            bitmap.recycle();
        }

        if (textureHandle[0] == 0)
        {
            throw new RuntimeException("Error loading texture.");
        }

        return textureHandle[0];
    }

    @Override
    public void onSurfaceTextureAvailable(int texName, boolean isExternal) {
        mTexName = texName;
        mIsExternal = isExternal;
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT
                | GLES20.GL_DEPTH_BUFFER_BIT);

        if (mTexName != -1) {
            if (!mIsInited) {
                if (mIsExternal) {
                    mProgram = ShaderHelper.createProgram(vertexShaderCode, extFragmentShaderCode);
                } else {
                    mProgram = ShaderHelper.createProgram(vertexShaderCode, fragmentShaderCode);
                }
                GLES20.glUseProgram(mProgram);

                mVertCount = 6;

                FloatBuffer vertexBuffer = ByteBuffer.allocateDirect(squareCoords.length * BYTES_PER_FLOAT)
                        .order(ByteOrder.nativeOrder()).asFloatBuffer();
                vertexBuffer.put(squareCoords);
                vertexBuffer.position(0);

//                GLES20.glGenBuffers(1, mVbo, 0);
//                GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mVbo[0]);
//                GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, vertexBuffer.capacity() * BYTES_PER_FLOAT, vertexBuffer, GLES20.GL_STATIC_DRAW);

                vertexBuffer.position(0);
                GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(mProgram, "vPosition"));
                GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(mProgram, "vPosition"), 3, GLES20.GL_FLOAT, false, BYTES_PER_FLOAT * 5, vertexBuffer);

                vertexBuffer.position(3);
                GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(mProgram, "vTexcoord"));
                GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(mProgram, "vTexcoord"), 2, GLES20.GL_FLOAT, false, BYTES_PER_FLOAT * 5, vertexBuffer);

                mIsInited = true;
            }

            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);

            if (mIsExternal) {
                GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTexName);
            } else {
                GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTexName);
            }

            GLES20.glUniform1i(GLES20.glGetUniformLocation(mProgram, "texture"), 0);
            GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, mVertCount);
        }
    }

    @Override
    public void onSurfaceDestroy() {

    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.back_button) {

            mMediaPlayer.stop();
            mMediaPlayer.releaseAll();

            this.finish();
        }
    }

}

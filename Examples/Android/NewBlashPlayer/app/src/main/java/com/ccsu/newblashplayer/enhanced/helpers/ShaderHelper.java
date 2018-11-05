package com.ccsu.newblashplayer.enhanced.helpers;

import android.opengl.GLES20;
import android.util.Log;

public class ShaderHelper {
    private static final String TAG = "ShaderHelper";

    public static int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        if (shader == 0) {
            Log.e(TAG, "create shader error : " + type);
            return -1;
        }

        int compiled[] = new int[1];
        // load the shader source
        GLES20.glShaderSource(shader, shaderCode);

        // compile the shader
        GLES20.glCompileShader(shader);

        // check the compile status
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
        if (compiled[0] == 0) {
            Log.e(TAG, "Error compile shader : " + GLES20.glGetShaderInfoLog(shader));
            GLES20.glDeleteShader(shader);
            return -1;
        }

        return shader;
    }

    public static int createProgram(final String vertexShaderCode, final String fragmentShaderCode) {
        // load the vertex shader
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        if (vertexShader == -1) {
            return -1;
        }

        // load the fragment shader
        int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);
        if (fragmentShader == -1) {
            return -1;
        }

        int program = GLES20.glCreateProgram();
        if (program == 0) {
            Log.e(TAG, "Error create program");
            return -1;
        }

        GLES20.glAttachShader(program, vertexShader);
        GLES20.glAttachShader(program, fragmentShader);
        GLES20.glLinkProgram(program);

        int[] linked = new int[1];
        // Check the link status
        GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linked, 0);
        if (linked[0] == 0) {
            GLES20.glDeleteProgram(program);

            GLES20.glDeleteShader(vertexShader);
            GLES20.glDeleteShader(fragmentShader);

            Log.e(TAG, "Error linking program: " +
                    GLES20.glGetProgramInfoLog(program));
            return -1;
        }

        GLES20.glDeleteShader(vertexShader);
        GLES20.glDeleteShader(fragmentShader);

        return program;
    }

    public static void destoryProgram(int program) {
        if (program <= 0) {
            return ;
        }
        GLES20.glDeleteProgram(program);
    }

}
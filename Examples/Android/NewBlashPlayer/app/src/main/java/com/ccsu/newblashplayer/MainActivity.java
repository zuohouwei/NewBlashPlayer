package com.ccsu.newblashplayer;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.Button;

import com.ccsu.nbmediaplayer.NBAVPlayer;
import com.ccsu.nbmediaplayer.view.NBGenericGLView;

import java.io.IOException;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = MainActivity.class.getSimpleName();

    private Button mGenericButton = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mGenericButton = this.findViewById(R.id.generic_button);
        mGenericButton.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.generic_button) {
            Intent intent = new Intent(this, GenericActivity.class);
            startActivity(intent);
        }
    }
}

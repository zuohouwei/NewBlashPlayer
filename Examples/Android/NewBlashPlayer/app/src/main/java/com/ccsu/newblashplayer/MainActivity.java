package com.ccsu.newblashplayer;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.ccsu.newblashplayer.enhanced.EnhancedActivity;
import com.ccsu.newblashplayer.generic.GenericActivity;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = MainActivity.class.getSimpleName();

    private Button mGenericButton = null;
    private Button mEnhancedButton = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mGenericButton = this.findViewById(R.id.generic_button);
        mGenericButton.setOnClickListener(this);

        mEnhancedButton = this.findViewById(R.id.enhanced_button);
        mEnhancedButton.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.generic_button) {
            Intent intent = new Intent(this, GenericActivity.class);
            startActivity(intent);
        } else if (v.getId() == R.id.enhanced_button) {
            Intent intent = new Intent(this, EnhancedActivity.class);
            startActivity(intent);
        }
    }
}

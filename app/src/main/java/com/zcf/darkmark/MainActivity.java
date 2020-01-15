package com.zcf.darkmark;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        final String deviceInfo = Build.DEVICE;
        final String rootPath = Environment.getExternalStorageDirectory().getPath();

        final EditText fileText = findViewById(R.id.input_file);
        final TextView info = findViewById(R.id.info);
        final TextView time = findViewById(R.id.time);

        Button btnWrite = findViewById(R.id.btn_write);
        Button btnRead = findViewById(R.id.btn_read);
        Button btnTime = findViewById(R.id.btn_time);
        btnWrite.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.e(TAG, "onClick write: " + fileText.getText().toString());
                final String fileName = rootPath + "/" + fileText.getText().toString();
                writeDeviceInfo(fileName, deviceInfo);
            }
        });
        btnRead.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.e(TAG, "onClick read: " + fileText.getText().toString());
                final String fileName = rootPath + "/" + fileText.getText().toString();
                String getInfo = readDeviceInfo(fileName);
                info.setText(getInfo);
            }
        });
        btnTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.e(TAG, "onClick: time");
                final String fileName = rootPath + "/" + fileText.getText().toString();
                time.setText(readFileTime(fileName));
            }
        });
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void writeDeviceInfo(String filename, String info);
    public native String readDeviceInfo(String filename);
    public native String readFileTime(String filename);
}

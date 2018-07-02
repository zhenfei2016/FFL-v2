package demo.player.ffl.androidplayer;

import android.Manifest;
import android.content.pm.PackageManager;
import android.media.AudioTrack;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;

import ffl.player.FFLPlayer;

public class MainActivity extends AppCompatActivity {
    TextureView mTextureView;
    FFLPlayer mFFLPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        requestPerssion();

        mTextureView=(TextureView)findViewById(R.id.video);
        View v=findViewById(R.id.btnPlay);
        v.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                if(mFFLPlayer==null) {
                    mFFLPlayer = new FFLPlayer();
                    mFFLPlayer.native_setSurface(new Surface(mTextureView.getSurfaceTexture()));
                }

                String url="/sdcard/DCIM/sintel.ts";
                mFFLPlayer.native_play(url);
            }
        });

        v=findViewById(R.id.btnStop);
        v.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                if(mFFLPlayer!=null) {
                    mFFLPlayer.native_stop();
                }
            }
        });
    }

    private void requestPerssion(){
        //判断用户是否已经授权，未授权则向用户申请授权，已授权则直接进行呼叫操作
        //
        if(ContextCompat.checkSelfPermission(
                MainActivity.this,"Manifest.permission.WRITE_EXTERNAL_STORAGE")
                != PackageManager.PERMISSION_GRANTED)
        {
            //注意第二个参数没有双引号
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},1);
        }
    }

    private void test(){
        AudioTrack audioTrack=null;

       // audioTrack.setStereoVolume()
    }
}

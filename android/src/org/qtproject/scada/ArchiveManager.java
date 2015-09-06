package org.qtproject.scada;

import java.io.*;
import java.lang.String;
import java.util.Iterator;
import java.util.List;
import android.app.Activity;
import android.content.ContentValues;
import android.content.res.Configuration;
import android.hardware.Camera;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.LinearLayout;

public class ArchiveManager {

    public ArchiveManager(){};
    static boolean flag = false;
    public static String newFile(String s){
        try{
            File nFile = new File(s);
            if(!nFile.exists())
            {
                flag = nFile.createNewFile();
            }
            else
                flag = true;


        }catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e){
            e.printStackTrace();
        }

        if(flag == true)
            return "success "+s;
        else
            return "failed "+s;
    }

   public String getStr()
   {
       return "13aaa";
   }

}


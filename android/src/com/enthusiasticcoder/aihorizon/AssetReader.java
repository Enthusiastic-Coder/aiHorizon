package com.enthusiasticcoder.aihorizon;

import android.content.Context;
import android.content.res.AssetManager;
import java.io.IOException;
import java.io.InputStream;

public class AssetReader {
    public static byte[] readAsset(Context context, String assetPackName, String fileName) {
        AssetManager assetManager = context.getAssets();
        String fullPath = assetPackName + "/" + fileName;

        try (InputStream inputStream = assetManager.open(fullPath)) {
            int size = inputStream.available();
            byte[] buffer = new byte[size];
            inputStream.read(buffer);
            return buffer;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }
}

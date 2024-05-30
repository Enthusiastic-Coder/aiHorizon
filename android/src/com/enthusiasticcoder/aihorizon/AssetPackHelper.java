package com.enthusiasticcoder.aihorizon;

import android.content.Context;
import android.content.res.AssetManager;

import com.google.android.play.core.assetpacks.AssetPackManager;
import com.google.android.play.core.assetpacks.AssetPackManagerFactory;
import com.google.android.play.core.assetpacks.AssetLocation;
import com.google.android.play.core.assetpacks.AssetPackStates;

import java.io.IOException;
import java.io.InputStream;

public class AssetPackHelper {
    private Context context;
    private AssetPackManager assetPackManager;

    public AssetPackHelper(Context context) {
        this.context = context;
        assetPackManager = AssetPackManagerFactory.getInstance(context);
    }


    public InputStream getObbFileInputStream(String packName, String assetName) throws IOException {
        AssetLocation assetLocation = assetPackManager.getAssetLocation (packName,assetName);
        if (assetLocation != null) {
            String assetPath = assetLocation.path();
            long offset = assetLocation.offset();
            long size = assetLocation.size();

            AssetManager assetManager = context.getAssets();

            try {
                InputStream inputStream = assetManager.openFd(assetPath).createInputStream();
                inputStream.skip(offset);

                return inputStream;
            }
            catch( IOException e)
            {
            }
        }
        return null;
    }
}

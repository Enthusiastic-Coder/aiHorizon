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

    public String getAssetPackPath(String packName, String assetName) {
        AssetLocation assetLocation = assetPackManager.getAssetLocation (packName,assetName);
        if (assetLocation != null) {
            return assetLocation.path();
        }
        return "";
    }

    public AssetLocation getAssetLocation(String packName, String fileName) {
        return assetPackManager.getAssetLocation(packName, fileName);
    }
}

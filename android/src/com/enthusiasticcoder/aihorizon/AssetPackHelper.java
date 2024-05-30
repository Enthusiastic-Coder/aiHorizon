package com.enthusiasticcoder.aihorizon;

import android.content.Context;
import android.content.res.AssetManager;

import com.google.android.play.core.assetpacks.AssetPackManager;
import com.google.android.play.core.assetpacks.AssetPackManagerFactory;
import com.google.android.play.core.assetpacks.AssetLocation;
import com.google.android.play.core.assetpacks.AssetPackLocation;
import com.google.android.play.core.assetpacks.AssetPackStates;

import java.io.IOException;
import java.io.InputStream;

public class AssetPackHelper {
    private AssetPackManager assetPackManager;

    public AssetPackHelper(Context context) {
        assetPackManager = AssetPackManagerFactory.getInstance(context);
    }

    public AssetLocation getAssetLocation(String packName, String fileName) {
        return assetPackManager.getAssetLocation(packName, fileName);
    }
}

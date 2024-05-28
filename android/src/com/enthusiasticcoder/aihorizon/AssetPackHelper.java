package com.enthusiasticcoder.aihorizon;

import android.content.Context;
import com.google.android.play.core.assetpacks.AssetPackManager;
import com.google.android.play.core.assetpacks.AssetPackManagerFactory;

public class AssetPackHelper {
    private AssetPackManager assetPackManager;

    public AssetPackHelper(Context context) {
        assetPackManager = AssetPackManagerFactory.getInstance(context);
    }

    public void requestAssetPack(String packName) {
        assetPackManager.fetch(java.util.Arrays.asList(packName));
    }
}

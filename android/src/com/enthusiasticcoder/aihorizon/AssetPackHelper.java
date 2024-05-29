package com.enthusiasticcoder.aihorizon;

import android.content.Context;
import com.google.android.play.core.assetpacks.AssetPackManager;
import com.google.android.play.core.assetpacks.AssetPackManagerFactory;
import com.google.android.play.core.assetpacks.AssetLocation;
import com.google.android.play.core.assetpacks.AssetPackStates;

public class AssetPackHelper {
    private AssetPackManager assetPackManager;

    public AssetPackHelper(Context context) {
        assetPackManager = AssetPackManagerFactory.getInstance(context);
    }


    public String getAssetPackPath(String packName, String assetName) {
        AssetLocation assetLocation = assetPackManager.getAssetLocation (packName,assetName);
        if (assetLocation != null) {
            return assetLocation.path();
        }
        return "";
    }
}

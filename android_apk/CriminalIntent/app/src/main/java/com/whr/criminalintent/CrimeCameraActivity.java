package com.whr.criminalintent;

import android.os.Bundle;
import android.support.v4.app.Fragment;

public class CrimeCameraActivity extends SingleFragmentActivity {

    @Override
    protected Fragment createFragment() {
        return new CrimeCameraFragment();
    }
}

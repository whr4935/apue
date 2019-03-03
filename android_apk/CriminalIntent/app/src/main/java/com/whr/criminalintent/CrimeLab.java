package com.whr.criminalintent;

import android.content.Context;
import android.util.Log;

import org.json.JSONException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.UUID;

/**
 * Created by whr on 17-4-28.
 */
public class CrimeLab {
    private static final String TAG = "CrimeLab";
    private static final String FILENAME="crimes.json";
    private static CrimeLab sCrimeLab;
    private Context mAppContex;
    private ArrayList<Crime> mCrimes;
    private CrimeIntentJSONSerializer mSerializer;


    private CrimeLab(Context appContext) {
        mAppContex = appContext;
//        mCrimes = new ArrayList<Crime>();

        mSerializer = new CrimeIntentJSONSerializer(mAppContex, FILENAME);
        try {
            mCrimes = mSerializer.loadCrimes();
        } catch (Exception e) {
            mCrimes = new ArrayList<Crime>();
            Log.e(TAG, "Error loading crimes: ", e);
        }

//        for (int i = 0; i < 100; ++i) {
//            Crime c = new Crime();
//            c.setTitle("Crime #" + i);
//            c.setSolved(i % 2 == 0);
//            mCrimes.add(c);
//        }
    }

    public static CrimeLab get(Context c) {
        if (sCrimeLab == null) {
            sCrimeLab = new CrimeLab(c.getApplicationContext());
        }

        return sCrimeLab;
    }

    public ArrayList<Crime> getCrimes() {
        return mCrimes;
    }

    public Crime getCrime(UUID id) {
        for (Crime c : mCrimes) {
            if (c.getId().equals(id))
                return c;
        }
        return null;
    }

    public boolean saveCrimes() {
        try {
            mSerializer.saveCrimes(mCrimes);
            Log.d(TAG, "crimes saved to file");

            return true;
        } catch (JSONException e) {
            e.printStackTrace();
            return false;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    public void deleteCrime(Crime c) {
        mCrimes.remove(c);
    }

    public void deleteAll() {
        mCrimes.clear();
    }

    public void addCrime(Crime c) {
        mCrimes.add(c);
    }
}

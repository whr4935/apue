package com.whr.geoquiz;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

public class QuizActivity extends AppCompatActivity {
    private static final String TAG = "QuizActivity";
    private static final String KEY_INDEX="index";

    private Button mTrueButton;
    private Button mFalseButton;
    private ImageButton mNextButton;
    private Button mCheatButton;
    private TextView mQuestionTextView;
        private TrueFalse[] mQuestionBank = new TrueFalse[] {
            new TrueFalse(R.string.question_1, true),
        new TrueFalse(R.string.question_2, false),
        new TrueFalse(R.string.question_3, true),
        new TrueFalse(R.string.question_4, true),
    };
    private int mCurrentIndex = 0;
    private boolean mIsCheater;

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        Log.i(TAG, "onSaveInstaceState");
        outState.putInt(KEY_INDEX, mCurrentIndex);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_quiz);
        Log.d(TAG, "onCreate called");

        if (savedInstanceState != null) {
            mCurrentIndex = savedInstanceState.getInt(KEY_INDEX);
        }



        mTrueButton = (Button)findViewById(R.id.true_button);
        mFalseButton = (Button)findViewById(R.id.false_button);
        mCheatButton = (Button)findViewById(R.id.cheat_button);

        if (mTrueButton != null) {
            mTrueButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
//                    Toast.makeText(QuizActivity.this, R.string.toast_true, Toast.LENGTH_SHORT).show();
                    checkAnswer(true);
                }
            });
        }

        if (mFalseButton != null) {
            mFalseButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                  //  Toast.makeText(QuizActivity.this, R.string.toast_false, Toast.LENGTH_SHORT).show();
                    checkAnswer(false);
                }
            });
        }

        mQuestionTextView = (TextView)findViewById(R.id.question_text_view);

        mNextButton = (ImageButton) findViewById(R.id.next_button);
        mNextButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mCurrentIndex = (mCurrentIndex+1)%mQuestionBank.length;
                mIsCheater = false;
               updateQuestion();
            }
        });

        mCheatButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //start CheatActivity
                Intent i = new Intent(QuizActivity.this, CheatActivity.class);
                i.putExtra(CheatActivity.EXTRA_ANSWER_IS_TRUE, mQuestionBank[mCurrentIndex].isTrueQuestion());
                startActivityForResult(i, 0);
            }
        });

        updateQuestion();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onAcitivityResult");
        if (data == null)
            return;
        mIsCheater = data.getBooleanExtra(CheatActivity.EXTRA_ANSWER_SHOWN, false);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "onPause called");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume called");

    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "onStart called");
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "onStop called");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy called");
    }

    private void updateQuestion() {

        int question = mQuestionBank[mCurrentIndex].getQuestion();
        mQuestionTextView.setText(question);
    }

    private void checkAnswer(boolean userAnswer) {
        boolean answer = mQuestionBank[mCurrentIndex].isTrueQuestion();

        if (mIsCheater) {
            Toast.makeText(this, R.string.judgment_toast, Toast.LENGTH_SHORT).show();
        } else if (userAnswer == answer) {
            Toast.makeText(this, R.string.toast_true, Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, R.string.toast_false, Toast.LENGTH_SHORT).show();
        }
    }
}

package com.whr.geoquiz;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

/**
 * Created by whr on 17-4-28.
 */

public class CheatActivity extends AppCompatActivity {
    public static  final String EXTRA_ANSWER_IS_TRUE="com.whr.geoquiz." +
            "anser_is_true";
    public static final String EXTRA_ANSWER_SHOWN = "com.whr.geoquiz.android." +
            "geoquiz.anser_shown";

    private boolean mAnswerIsTrue = false;
    private TextView mAnswerTextView;
    private Button mShowAnswer;
    private TextView mShowApiLevelView;

    private void setAnswerShownResult(boolean isAnswerShown) {
        Intent data = new Intent();
        data.putExtra(EXTRA_ANSWER_SHOWN, isAnswerShown);
        setResult(RESULT_OK, data);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cheat);

        mAnswerIsTrue = getIntent().getBooleanExtra(EXTRA_ANSWER_IS_TRUE, false);

        mAnswerTextView = (TextView)findViewById(R.id.answerTextView);
        mShowAnswer = (Button)findViewById(R.id.showAnserButton);

        mShowApiLevelView = (TextView)findViewById(R.id.apiLevel);
        mShowApiLevelView.setText(String.format("%d", Build.VERSION.SDK_INT));

        setAnswerShownResult(false);

        mShowAnswer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mAnswerIsTrue) {
                    mAnswerTextView.setText(R.string.toast_true);
                } else {
                    mAnswerTextView.setText(R.string.toast_false);
                }
                setAnswerShownResult(true);
            }
        });
    }
}

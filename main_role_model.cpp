#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int T;
    cin >> T;
    while (T--)
    {
        ull n, t;
        ull N;
        cin >> n >> t >> N;
        string s;
        cin >> s;
        // 初期状態をビット列から整数に変換
        ull state = 0;
        for (int i = 0; i < n; i++)
        {
            state = (state << 1) | (s[i] - '0');
        }
        // フィードバックタップのマスクを作成
        ull feedback_mask = 0;
        for (int i = 0; i < t; i++)
        {
            int k;
            cin >> k;
            feedback_mask |= (1ULL << k);
        }
        // LFSRの動作を定義するラムダ関数
        auto lfsr_step = [&](ull current_state) -> ull
        {
            // 最上位ビットを取得
            ull out = (current_state >> (n - 1)) & 1;
            // フィードバックビットを計算
            ull feedback = __builtin_popcount(current_state & feedback_mask) % 2;
            // シフト操作を実行
            current_state = ((current_state << 1) & ((1ULL << n) - 1)) | feedback;
            return current_state;
        };
        // 高速指数計算のために2^kステップの遷移を事前に計算
        // max_bitはNを2進数で表したときのビット数
        int max_bit = 0;
        ull temp_N = N;
        while (temp_N > 0)
        {
            temp_N >>= 1;
            max_bit++;
        }
        // 各bitに対応する遷移を格納する配列
        vector<ull> transitions(max_bit, 0);
        // transitions[0]は1ステップ後の状態
        transitions[0] = lfsr_step(state);
        // transitions[k]は2^kステップ後の状態
        for (int k = 1; k < max_bit; k++)
        {
            transitions[k] = lfsr_step(transitions[k - 1]);
        }
        // 初期状態をcurrent_stateにセット
        ull current_state = state;
        // Nを2進数で見て各ビットに応じて遷移を適用
        for (int k = 0; k < max_bit; k++)
        {
            if (N & (1ULL << k))
            {
                // kビット目が立っている場合、2^kステップ遷移を適用
                // transitions[k]は2^kステップ後の状態
                current_state = transitions[k];
            }
        }
        // 最終状態をビット列に変換
        string res = "";
        for (int i = n - 1; i >= 0; i--)
        {
            res += ((current_state & (1ULL << i)) ? '1' : '0');
        }
        cout << res << "\n";
    }
}

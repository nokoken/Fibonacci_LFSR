#include <iostream>
#include <bit>
#include <vector>
using ull = unsigned long long;

int main()
{
    ull n; // ビット数
    ull t; // フィードバックタップの数
    ull N; // 線形帰還シフトレジスタのステップ数
    std::cin >> n >> t >> N;
    std::string input_bits; // 入力ビット
    std::cin >> input_bits;
    // 文字列で取ってきた入力ビットを数字に変換
    ull state = 0;
    for (int i = 0; i < n; i++)
        state = (state << 1) | (input_bits[i] - '0');
    // フィードバックタップのマスクを作成
    ull feedback_mask = 0;
    for (int i = 0; i < t; i++)
    {
        int k;
        std::cin >> k;
        feedback_mask |= (1ULL << k);
    }
    // LFSRの動作を定義するラムダ式
    auto lfsr_step = [&](ull current_state) -> ull
    {
        // 最上位ビット
        ull out = (current_state >> n - 1) & 1;
        // フィードバックタップ使用後のビットの数を見る
        ull feedback = __builtin_popcount(current_state & feedback_mask) % 2;
        current_state = ((current_state << 1) & (1ULL << n) - 1) | feedback;
        return current_state;
    };
    // 2^Nの計算を計算量削減のために先に処理しておく
    int max_bit = 0;
    ull temp_N = N;
    while (temp_N > 0)
    {
        temp_N >>= 1;
        max_bit++;
    }
    // 各ビットの遷移を格納する配列
    std::vector<ull> transitions(max_bit, 0);
    // ステップ1
    transitions[0] = lfsr_step(state);
    // ステップ2~max_bitまで
    for (int k = 1; k < max_bit; k++)
        transitions[k] = lfsr_step(transitions[k - 1]);
    ull current_state = state;
    for (int k = 0; k < max_bit; k++)
    {
        std::string res = "";
        if (N & (1ULL << k))
            current_state = transitions[k];
        for (int i = n - 1; i >= 0; i--)
            res += ((current_state & (1ULL << i)) ? '1' : '0');
        std::cout << res << std::endl;
    }
}
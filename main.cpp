#include <iostream>
#include <string>
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
        // フィードバックタップ使用後のビットの数を見る
        ull feedback = __builtin_popcount(current_state & feedback_mask) % 2;
        current_state = ((current_state << 1) & (1ULL << n) - 1) | feedback;
        return current_state;
    };
    // 各ビットの遷移を格納する配列
    std::vector<ull> transitions(N, 0);
    // ステップ1
    transitions[0] = lfsr_step(state);
    // ステップ2~Nまで
    for (int k = 1; k < N; k++)
        transitions[k] = lfsr_step(transitions[k - 1]);
    std::string res = "";
    for (int k = 0; k < N; k++)
    {
        res += std::to_string(k + 1) + "番目の遷移は";
        for (int i = n - 1; i >= 0; i--)
            res += ((transitions[k] & (1ULL << i)) ? '1' : '0');
        res += "です\n";
    }
    std::cout << res << std::endl;
}
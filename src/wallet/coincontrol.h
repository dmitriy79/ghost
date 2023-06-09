// Copyright (c) 2011-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_WALLET_COINCONTROL_H
#define BITCOIN_WALLET_COINCONTROL_H

#include <optional.h>
#include <outputtype.h>
#include <policy/feerate.h>
#include <policy/fees.h>
#include <primitives/transaction.h>
#include <script/standard.h>
#include <key.h>
#include <pubkey.h>


class CCoinControlEntry
{
public:
    COutPoint op;
    int nType;
    int nDepth;
    CAmount nValue;
    CScript scriptPubKey;
    int64_t nTxTime;
};

class CInputData
{
public:
    CAmount nValue;
    uint256 blind;
    CScriptWitness scriptWitness;
    secp256k1_pedersen_commitment commitment;
    CCmpPubKey pubkey;
    CKey privkey;
    OutputTypes nType{OUTPUT_STANDARD};
};

const int DEFAULT_MIN_DEPTH = 0;
const int DEFAULT_MAX_DEPTH = 9999999;

//! Default for -avoidpartialspends
static constexpr bool DEFAULT_AVOIDPARTIALSPENDS = false;

/** Coin Control Features. */
class CCoinControl
{
public:
    //! Custom change destination, if not set an address is generated
    CTxDestination destChange;
    //! Override the default change type if set, ignored if destChange is set
    Optional<OutputType> m_change_type;
    //! If false, only selected inputs are used
    bool m_add_inputs;
    //! If false, allows unselected inputs, but requires all selected inputs be used
    bool fAllowOtherInputs;
    //! Includes watch only addresses which are solvable
    bool fAllowWatchOnly;
    //! Override automatic min/max checks on fee, m_feerate must be set if true
    bool fOverrideFeeRate;

    //! Override the wallet's m_pay_tx_fee if set
    Optional<CFeeRate> m_feerate;
    //! Override the default confirmation target if set
    Optional<unsigned int> m_confirm_target;
    //! Override the wallet's m_signal_rbf if set
    Optional<bool> m_signal_bip125_rbf;
    //! Avoid partial use of funds sent to a given address
    bool m_avoid_partial_spends;
    //! Forbids inclusion of dirty (previously used) addresses
    bool m_avoid_address_reuse;
    //! Fee estimation mode to control arguments to estimateSmartFee
    FeeEstimateMode m_fee_mode;
    //! Minimum chain depth value for coin availability
    int m_min_depth = DEFAULT_MIN_DEPTH;
    //! Maximum chain depth value for coin availability
    int m_max_depth = DEFAULT_MAX_DEPTH;

    //! Particl
    int nCoinType;
    mutable bool fHaveAnonOutputs = false;
    mutable bool fNeedHardwareKey = false;
    CAmount m_extrafee;
    std::map<COutPoint, CInputData> m_inputData;
    bool fAllowLocked = false;
    mutable int nChangePos = -1;
    bool m_addChangeOutput = true;
    bool m_include_immature = false;
    //! Allows amounts of blinded outputs sent to stealth addresses to be seen with the scan_secret
    bool m_blind_watchonly_visible = false;
    //! Appended to ct fee data output
    std::vector<uint8_t> m_extra_data0;
    //! Allow spending frozen blinded outputs
    bool m_spend_frozen_blinded = false;
    //! Include non whitelisted outputs
    bool m_include_tainted_frozen = false;
    //! Trigger rct mint exploit for tests, increase by amount
    CAmount m_debug_exploit_anon = 0;
    //! Vector of mixins to use
    std::vector<int64_t> m_use_mixins;
    //! mixin selection mode to use: 1 select from range, 2 select near real index
    int m_mixin_selection_mode = 1;
    //! Blinding factor for input amount commitment when > 1 mlsag
    mutable std::vector<CKey> vSplitCommitBlindingKeys;
    //! Script to use for the change output
    CScript scriptChange;
    //! Pubkey to use for the change output if changeaddress and scriptChange isn't set
    CPubKey m_changepubkey;
    //! Minimum value of outputs to select
    CAmount m_minimum_output_amount = 0;
    //! Maximum value of outputs to select
    CAmount m_maximum_output_amount = MAX_MONEY;

    CCoinControl()
    {
        SetNull();
    }

    void SetNull();

    bool HasSelected() const
    {
        return (setSelected.size() > 0);
    }

    bool IsSelected(const COutPoint& output) const
    {
        return (setSelected.count(output) > 0);
    }

    void Select(const COutPoint& output)
    {
        setSelected.insert(output);
    }

    void UnSelect(const COutPoint& output)
    {
        setSelected.erase(output);
    }

    void UnSelectAll()
    {
        setSelected.clear();
    }

    void ListSelected(std::vector<COutPoint>& vOutpoints) const
    {
        vOutpoints.assign(setSelected.begin(), setSelected.end());
    }

    size_t NumSelected()
    {
        return setSelected.size();
    }

    bool SetKeyFromInputData(const CKeyID &idk, CKey &key) const;

//private:
    std::set<COutPoint> setSelected;
};

#endif // BITCOIN_WALLET_COINCONTROL_H

#include <prove.hpp>

public_key ecverify(name data, const signature &sig, const public_key &pubkey)
 {
   checksum256 digest = sha256(data.to_string().c_str(), data.to_string().size());
   assert_recover_key(digest, sig, pubkey);
   return pubkey; 
 }

bool not_paused(){
  return true;
}

bool is_locked(name account){
  return false;
}

bool is_enabled(name action){
  return true;
}



ACTION prove::loadclaim(name worbli_account, asset edna_amount, public_key pubkey) {
  require_auth(get_self());
  check(not_paused(), "this contract has been paused - please try again later");
  auto sym = edna_amount.symbol;
  check(sym.is_valid(), "invalid symbol name");
  //stats statstable(get_self(), sym.code().raw());
  //auto existing = statstable.find(sym.code().raw());
  //check(existing != statstable.end(), "token with symbol does not exist");
  //const auto &st = *existing;
  check(edna_amount.is_valid(), "invalid quantity");
  check(edna_amount.amount > 0, "must load a positive quantity claim");
  //check(edna_amount.symbol == st.supply.symbol, "symbol precision mismatch");
  //statstable.modify(st, same_payer, [&](auto &s) {
  //  s.supply += edna_amount;
 // });
  worbliclaims_table w_t(get_self(),get_self().value);
  w_t.emplace(get_self(), [&](auto & w) {
    w.worbli_account = worbli_account;
    w.edna_amount = edna_amount;
    w.pubkey = pubkey;
  });
}

ACTION prove::worbclaim(name ux_account, name worbli_account, signature sig) {
  check(not_paused(), "this contract has been paused - please try again later");
  check(is_locked(ux_account) == false, "this account is locked - visit https://t.me/edna_life");
  check(is_enabled("worbclaim"_n) == true,"claiming Worbli-based EDNA is disabled - please try again later");

  worbliclaims_table worbli_table(get_self(), get_self().value);
  //name find_worbli = name(worbli_account);
  auto w_itr = worbli_table.find(worbli_account.value);
  check( w_itr != worbli_table.end(), "account does not exist in claims table" );
  //asset send_amount = w_itr->edna_amount;
  public_key snap_key = w_itr->pubkey;

  public_key tmp = ecverify(worbli_account, sig, snap_key);  //see if the snapshot stored key is the same as the signature
  
  check(snap_key == tmp, "invalid signature");

    // earase the claim and credit the tokens
 //   w_itr = w_t.erase(w_itr);
    //add_balance( ux_account, send_amount, ux_account );
 // }
}

ACTION prove::endclaim() {
  require_auth(get_self());
  //TO-DO - diable the cmail command and erase the claim table
  //move the unclaimed tokens to the ladder fund
}

ACTION prove::clear() {
  require_auth(get_self());

  worbliclaims_table worbli_table(get_self(), get_self().value);
  
  while(worbli_table.begin() != worbli_table.end()){
    worbli_table.erase(worbli_table.begin());
  }


}
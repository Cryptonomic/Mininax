module CountedTransactions = {
  let json_of_magic = magic => magic |> Obj.magic |> Js.Json.object_;
  let hash = json =>
    json
    |> Json.Decode.field("count_operation_group_hash", Json.Decode.string)
    |> int_of_string;
};
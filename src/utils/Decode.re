let json_of_magic = magic => magic |> Obj.magic |> Js.Json.object_;

let countedTransactions = json =>
  json
  |> Json.Decode.field("count_operation_group_hash", Json.Decode.string)
  |> int_of_string;

let countedZeroPriorityBlocksLevels = json =>
  json
  |> Json.Decode.field("count_level", Json.Decode.string)
  |> int_of_string;

let countedBaker = json =>
  json
  |> Json.Decode.field("count_baker", Json.Decode.string)
  |> int_of_string;
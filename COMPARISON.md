# TF-Encrypted

![app-stack](app-stack.png)

**Underlying Protocols:** ABY3, SecureNN, Pond

## Strength

- Providing high-level language interfaces. Non-expert users can utilize the framework with python language and corresponding APIs.
- Supporting multiple privacy-preserving paradigms, including Federate learning and Multi-party Learning.

## Weakness

- Only applicable in two or three parties scenarios.
- Cannot support complex models (e.g., LSTM).
- Cannot add or remove training parties dynamically.







# SecMML

![MPL-arch](MPL-arch.png)

**Underlying Protocol:** BGW

## Strength

- Supporting an arbitrary number of data owners who participate in the collaborative training.
- Enabling users to conduct multiple intelligent models, such as Long-Short Term Memory (LSTM), through simply configuring a data flow graph.
- Adding or removing training parties dynamically.
- No dependence on any external library and can be easily deployed.
- Promising efficiency.

## Summary

|                                   | <font size =2>Party Number</font> | <font size =2>Data Flow Graph</font> | <font size =2>Dynamically add/remove parties</font> | <font size =2>Independent of external libraries</font> | <font size =2>Complex Models (e.g. LSTM)</font> |
| --------------------------------- | --------------------------------- | ------------------------------------ | --------------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------- |
| SecMML                            | 3+                                | Yes                                  | Yes                                                 | Yes                                                    | Yes                                             |
| <font size =3>TF-Encrypted</font> | 2 or 3                            | Yes                                  | No                                                  | No                                                     | Currently No                                    |


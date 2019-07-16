export interface Config {
  url: string;
  apiKey: string;
  platform: string;
  network: string;
  displayName: string;
}

export interface Field {
  name: string;
  displayName: string;
  isLink?: boolean;
}

export interface MatchParams {
  network?: string;
  entity?: string;
  id?: string;
}

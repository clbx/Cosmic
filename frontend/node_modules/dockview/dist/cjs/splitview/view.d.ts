/// <reference types="react" />
import { SplitviewPanel } from 'dockview-core';
import { ReactPart, ReactPortalStore } from '../react';
import { ISplitviewPanelProps } from './splitview';
export declare class ReactPanelView extends SplitviewPanel {
    private readonly reactComponent;
    private readonly reactPortalStore;
    constructor(id: string, component: string, reactComponent: React.FunctionComponent<ISplitviewPanelProps>, reactPortalStore: ReactPortalStore);
    getComponent(): ReactPart<ISplitviewPanelProps>;
}

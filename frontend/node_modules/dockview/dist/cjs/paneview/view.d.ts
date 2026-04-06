import React from 'react';
import { PanelUpdateEvent, IPanePart, PanePanelComponentInitParameter } from 'dockview-core';
import { ReactPortalStore } from '../react';
import { IPaneviewPanelProps } from './paneview';
export declare class PanePanelSection implements IPanePart {
    readonly id: string;
    private readonly component;
    private readonly reactPortalStore;
    private readonly _element;
    private part?;
    get element(): HTMLElement;
    constructor(id: string, component: React.FunctionComponent<IPaneviewPanelProps>, reactPortalStore: ReactPortalStore);
    init(parameters: PanePanelComponentInitParameter): void;
    toJSON(): {
        id: string;
    };
    update(params: PanelUpdateEvent): void;
    dispose(): void;
}
